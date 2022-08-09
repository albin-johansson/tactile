#include "compression.hpp"

#include <algorithm>  // clamp

#include <spdlog/spdlog.h>
#include <zlib.h>

#include "meta/build.hpp"
#include "meta/profile.hpp"
#include "misc/assert.hpp"

namespace tactile {
namespace {

constexpr usize buffer_size = 32'768;  // About 32 KB for our temporary buffers

}  // namespace

auto zlib_compress(const void* source, const usize source_bytes, int level)
    -> Maybe<ZlibData>
{
  if (level != Z_DEFAULT_COMPRESSION) {
    level = std::clamp(Z_BEST_SPEED, level, Z_BEST_COMPRESSION);
  }

  if (!source || source_bytes == 0) {
    return nothing;
  }

  Bytef temp_buffer[buffer_size];

  z_stream stream {};
  stream.next_in = (Bytef*) source;
  stream.avail_in = static_cast<uInt>(source_bytes);
  stream.next_out = temp_buffer;
  stream.avail_out = sizeof temp_buffer;

  if (const auto res = deflateInit(&stream, level); res != Z_OK) {
    spdlog::error("Could not initialize z_stream for compression: {}", zError(res));
    return nothing;
  }

  const auto upper_bound = deflateBound(&stream, source_bytes);

  ZlibData dest;
  dest.reserve(upper_bound);

  auto copy_buffer_to_dest = [&] {
    const auto written_bytes = buffer_size - stream.avail_out;
    dest.insert(dest.end(), temp_buffer, temp_buffer + written_bytes);
  };

  int status {};
  do {
    status = deflate(&stream, Z_FINISH);

    if (status == Z_STREAM_END) {
      copy_buffer_to_dest();
    }
    else if (status == Z_OK || status == Z_BUF_ERROR) {
      // We ran out of space in the output buffer, so reuse it!
      copy_buffer_to_dest();
      stream.next_out = temp_buffer;
      stream.avail_out = sizeof temp_buffer;
    }
    else {
      spdlog::error("Compression error: {}", zError(status));
      return nothing;
    }
  } while (status != Z_STREAM_END);

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

  Bytef temp_buffer[buffer_size];

  z_stream stream {};
  stream.next_in = (Bytef*) source;
  stream.avail_in = static_cast<uInt>(source_bytes);
  stream.next_out = temp_buffer;
  stream.avail_out = sizeof temp_buffer;

  if (inflateInit(&stream) != Z_OK) {
    return nothing;
  }

  ZlibData dest;
  dest.reserve(2048);

  auto copy_buffer_to_dest = [&] {
    const auto written_bytes = buffer_size - stream.avail_out;
    dest.insert(dest.end(), temp_buffer, temp_buffer + written_bytes);
  };

  int status {};
  do {
    status = inflate(&stream, Z_FINISH);

    if (status == Z_STREAM_END) {
      copy_buffer_to_dest();
    }
    else if (status == Z_OK || status == Z_BUF_ERROR) {
      // We ran out of space in the output buffer, so reuse it!
      copy_buffer_to_dest();
      stream.next_out = temp_buffer;
      stream.avail_out = sizeof temp_buffer;
    }
    else {
      spdlog::error("Decompression error: {}", zError(status));
      return nothing;
    }
  } while (status != Z_STREAM_END);

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

}  // namespace tactile
