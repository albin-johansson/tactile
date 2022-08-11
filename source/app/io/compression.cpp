#include "compression.hpp"

#include <algorithm>  // clamp

#include <spdlog/spdlog.h>
#include <zlib.h>

#include "meta/build.hpp"
#include "meta/profile.hpp"
#include "misc/assert.hpp"

namespace tactile::io {
namespace {

using ZlibProcessFun = int (*)(z_stream*, int);

constexpr usize buffer_size = 32'768;  // About 32 KB for our temporary buffers

[[nodiscard]] auto process_chunks(z_stream&      stream,
                                  ZlibProcessFun process,
                                  Bytef*         out_buffer,
                                  ZlibData&      result) -> int
{
  const auto copy_written_bytes_in_buffer_to_dest = [&] {
    const auto written_bytes = buffer_size - stream.avail_out;
    result.insert(result.end(), out_buffer, out_buffer + written_bytes);
  };

  while (true) {
    const auto status = process(&stream, Z_FINISH);
    switch (status) {
      case Z_OK:
        [[fallthrough]];
      case Z_BUF_ERROR: {
        // We ran out of space in the output buffer, so reuse it!
        copy_written_bytes_in_buffer_to_dest();
        stream.next_out = out_buffer;
        stream.avail_out = buffer_size;
        break;
      }
      case Z_STREAM_END: {
        copy_written_bytes_in_buffer_to_dest();
        return status;
      }
      default: {
        // Something went wrong, so just return the error code
        return status;
      }
    }
  }
}

}  // namespace

auto zlib_compress(const void* source, const usize source_bytes, int level)
    -> Maybe<ZlibData>
{
  if (level != Z_DEFAULT_COMPRESSION) {
    level = std::clamp(level, Z_BEST_SPEED, Z_BEST_COMPRESSION);
  }

  if (!source || source_bytes == 0) {
    return nothing;
  }

  Bytef out_buffer[buffer_size];

  z_stream stream {};
  stream.next_in = (Bytef*) source;
  stream.avail_in = static_cast<uInt>(source_bytes);
  stream.next_out = out_buffer;
  stream.avail_out = sizeof out_buffer;

  if (const auto res = deflateInit(&stream, level); res != Z_OK) {
    spdlog::error("Could not initialize z_stream for compression: {}", zError(res));
    return nothing;
  }

  ZlibData dest;
  dest.reserve(deflateBound(&stream, source_bytes));

  if (const auto status = process_chunks(stream, deflate, out_buffer, dest);
      status != Z_STREAM_END) {
    spdlog::error("Compression error: {}", zError(status));
    return nothing;
  }

#if TACTILE_DEBUG
  uint       pending {};
  int        pending_bits {};
  const auto pending_state = deflatePending(&stream, &pending, &pending_bits);
  TACTILE_ASSERT(pending_state == Z_OK);
  TACTILE_ASSERT(pending == 0);
  TACTILE_ASSERT(pending_bits == 0);
#endif  // TACTILE_DEBUG

  if (const auto res = deflateEnd(&stream); res != Z_OK) {
    spdlog::error("Could not tear down compression stream: {}", zError(res));
    return nothing;
  }

  return dest;
}

auto zlib_decompress(const void* source, const usize source_bytes) -> Maybe<ZlibData>
{
  if (!source || source_bytes == 0) {
    return nothing;
  }

  Bytef out_buffer[buffer_size];

  z_stream stream {};
  stream.next_in = (Bytef*) source;
  stream.avail_in = static_cast<uInt>(source_bytes);
  stream.next_out = out_buffer;
  stream.avail_out = sizeof out_buffer;

  if (const auto res = inflateInit(&stream); res != Z_OK) {
    spdlog::error("Could not initialize z_stream for decompression: {}", zError(res));
    return nothing;
  }

  ZlibData dest;
  dest.reserve(2048);

  if (const auto status = process_chunks(stream, inflate, out_buffer, dest);
      status != Z_STREAM_END) {
    spdlog::error("Decompression error: {}", zError(status));
    return nothing;
  }

  if (const auto res = inflateEnd(&stream); res != Z_OK) {
    spdlog::error("Could not tear down decompression stream: {}", zError(res));
    return nothing;
  }

  return dest;
}

auto zlib_decompress(std::span<const uint8> span) -> Maybe<ZlibData>
{
  return zlib_decompress(span.data(), span.size_bytes());
}

}  // namespace tactile::io
