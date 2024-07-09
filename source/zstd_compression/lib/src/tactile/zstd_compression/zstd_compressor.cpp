// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/zstd_compression/zstd_compressor.hpp"

#include <algorithm>  // copy_n
#include <iterator>   // back_inserter

#include <zstd.h>

#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/runtime/runtime.hpp"

namespace tactile {
namespace zstd_compressor_impl {

struct DStreamDeleter final
{
  void operator()(ZSTD_DStream* stream) noexcept
  {
    ZSTD_freeDStream(stream);
  }
};

using UniqueDStream = Unique<ZSTD_DStream, DStreamDeleter>;

}  // namespace zstd_compressor_impl

auto ZstdCompressor::compress(const ByteSpan input_data) const
    -> Result<ByteStream>
{
  const auto compression_bound = ZSTD_compressBound(input_data.size_bytes());

  ByteStream compressed_data {};
  compressed_data.resize(compression_bound);

  const auto written_byte_count = ZSTD_compress(compressed_data.data(),
                                                compressed_data.size(),
                                                input_data.data(),
                                                input_data.size_bytes(),
                                                ZSTD_CLEVEL_DEFAULT);

  if (ZSTD_isError(written_byte_count)) {
    Runtime::log(LogLevel::kError,
                 "Compression failed: {}",
                 ZSTD_getErrorName(written_byte_count));
    return unexpected(std::make_error_code(std::errc::io_error));
  }

  compressed_data.resize(written_byte_count);
  compressed_data.shrink_to_fit();

  return compressed_data;
}

auto ZstdCompressor::decompress(const ByteSpan input_data) const
    -> Result<ByteStream>
{
  const zstd_compressor_impl::UniqueDStream stream {ZSTD_createDStream()};
  if (!stream) {
    Runtime::log(LogLevel::kError, "Could not create stream");
    return unexpected(std::make_error_code(std::errc::not_enough_memory));
  }

  const auto init_stream_result = ZSTD_initDStream(stream.get());
  if (ZSTD_isError(init_stream_result)) {
    Runtime::log(LogLevel::kError,
                 "Could not initialize stream: {}",
                 ZSTD_getErrorName(init_stream_result));
    return unexpected(std::make_error_code(std::errc::io_error));
  }

  const auto staging_buffer_size = ZSTD_DStreamOutSize();

  ByteStream staging_buffer {};
  staging_buffer.resize(staging_buffer_size);

  ByteStream decompressed_data {};
  decompressed_data.reserve(staging_buffer_size);

  ZSTD_inBuffer input_view {input_data.data(), input_data.size_bytes(), 0};
  ZSTD_outBuffer output_view {staging_buffer.data(), staging_buffer_size, 0};

  // Copies the contents of the staging buffer to the result byte stream.
  const auto flush_staging_buffer = [&] {
    std::copy_n(staging_buffer.data(),
                output_view.pos,
                std::back_inserter(decompressed_data));
  };

  usize byte_write_count = 0;

  do {
    // Check if our staging buffer is full, in which case we flush and reuse it.
    if (output_view.pos == output_view.size) {
      flush_staging_buffer();

      output_view.dst = staging_buffer.data();
      output_view.size = staging_buffer_size;
      output_view.pos = 0;
    }

    const auto decompress_result =
        ZSTD_decompressStream(stream.get(), &output_view, &input_view);

    if (ZSTD_isError(decompress_result)) {
      Runtime::log(LogLevel::kError,
                   "Decompression failed: {}",
                   ZSTD_getErrorName(decompress_result));
      return unexpected(std::make_error_code(std::errc::io_error));
    }

    byte_write_count += output_view.pos;
  } while (input_view.pos < input_view.size);

  flush_staging_buffer();

  decompressed_data.resize(byte_write_count);
  decompressed_data.shrink_to_fit();

  return decompressed_data;
}

}  // namespace tactile
