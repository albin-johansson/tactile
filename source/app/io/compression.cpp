/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "compression.hpp"

#include <algorithm>  // copy_n, clamp
#include <iterator>   // back_inserter

#include <spdlog/spdlog.h>
#include <zlib.h>
#include <zstd.h>

#include "core/predef.hpp"
#include "core/type/ptr.hpp"
#include "debug/assert.hpp"

namespace tactile::io {
namespace {

using ZlibProcessFun = int (*)(z_stream*, int);

constexpr usize buffer_size = 32'768;  // About 32 KB for our temporary buffers

[[nodiscard]] auto process_zlib_chunks(z_stream& stream,
                                       ZlibProcessFun process,
                                       Bytef* out_buffer,
                                       ByteStream& result) -> int
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
    -> Maybe<ByteStream>
{
  if (!source || source_bytes == 0) {
    return nothing;
  }

  if (level != Z_DEFAULT_COMPRESSION) {
    level = std::clamp(level, Z_BEST_SPEED, Z_BEST_COMPRESSION);
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

  ByteStream dest;
  dest.reserve(deflateBound(&stream, static_cast<uLong>(source_bytes)));

  if (const auto status = process_zlib_chunks(stream, deflate, out_buffer, dest);
      status != Z_STREAM_END) {
    spdlog::error("Compression error: {}", zError(status));
    return nothing;
  }

#if TACTILE_DEBUG
  uint pending {};
  int pending_bits {};
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

auto zlib_decompress(const void* source, const usize source_bytes) -> Maybe<ByteStream>
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

  ByteStream dest;
  dest.reserve(2048);

  if (const auto status = process_zlib_chunks(stream, inflate, out_buffer, dest);
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

auto zlib_decompress(Span<const uint8> span) -> Maybe<ByteStream>
{
  return zlib_decompress(span.data(), span.size_bytes());
}

auto zstd_compress(const void* source, const usize source_bytes) -> Maybe<ByteStream>
{
  if (!source || source_bytes == 0) {
    return nothing;
  }

  const auto dest_capacity = ZSTD_compressBound(source_bytes);

  ByteStream dest;
  dest.resize(dest_capacity);

  const auto written_bytes = ZSTD_compress(dest.data(),
                                           dest_capacity,
                                           source,
                                           source_bytes,
                                           ZSTD_CLEVEL_DEFAULT);

  if (ZSTD_isError(written_bytes)) {
    spdlog::error("Could not compress data using zstd: {}",
                  ZSTD_getErrorName(written_bytes));
    return nothing;
  }

  dest.resize(written_bytes);
  dest.shrink_to_fit();

  return dest;
}

auto zstd_decompress(const void* source, const usize source_bytes) -> Maybe<ByteStream>
{
  const auto stream_deleter = [](ZSTD_DStream* ptr) noexcept { ZSTD_freeDStream(ptr); };

  using DStream = Unique<ZSTD_DStream, decltype(stream_deleter)>;
  DStream stream {ZSTD_createDStream()};

  if (!stream) {
    spdlog::error("Could not initialize zstd stream for decompression!");
    return nothing;
  }

  ZSTD_initDStream(stream.get());

  const auto out_buffer_size = ZSTD_DStreamOutSize();

  ByteStream out_buffer;
  out_buffer.reserve(out_buffer_size);

  ZSTD_inBuffer input {.src = source, .size = source_bytes, .pos = 0};
  ZSTD_outBuffer output {.dst = out_buffer.data(), .size = out_buffer_size, .pos = 0};

  ByteStream result;
  result.reserve(out_buffer_size);  // Won't be enough, but better than nothing

  const auto copy_output_buffer_to_result = [&] {
    std::copy_n(out_buffer.data(), output.pos, std::back_inserter(result));
  };

  usize written_bytes = 0;

  do {
    // Check if our output buffer is full, in which case we reset and reuse the buffer
    if (output.pos == output.size) {
      copy_output_buffer_to_result();

      output.dst = out_buffer.data();
      output.size = out_buffer_size;
      output.pos = 0;
    }

    const auto err = ZSTD_decompressStream(stream.get(), &output, &input);
    if (ZSTD_isError(err)) {
      spdlog::error("zstd decompression step failed: {}", ZSTD_getErrorName(err));
      return nothing;
    }

    written_bytes += output.pos;
  } while (input.pos < input.size);

  copy_output_buffer_to_result();

  result.resize(written_bytes);
  result.shrink_to_fit();

  return result;
}

auto zstd_decompress(Span<const uint8> span) -> Maybe<ByteStream>
{
  return zstd_decompress(span.data(), span.size_bytes());
}

}  // namespace tactile::io
