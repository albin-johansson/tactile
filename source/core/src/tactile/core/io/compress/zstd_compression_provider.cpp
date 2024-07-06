// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/compress/zstd_compression_provider.hpp"

#include <algorithm>  // copy_n

#include <zstd.h>

#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/core/io/compress/compression_error.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/set_log_scope.hpp"

namespace tactile {
inline namespace zstd_compression_provider {

struct DStreamDeleter final
{
  void operator()(ZSTD_DStream* stream) noexcept
  {
    ZSTD_freeDStream(stream);
  }
};

using UniqueDStream = Unique<ZSTD_DStream, DStreamDeleter>;

}  // namespace zstd_compression_provider

auto ZstdCompressionProvider::compress(const ByteSpan input_data) const
    -> Result<ByteStream>
{
  const SetLogScope log_scope {"ZstdCompressionProvider"};

  const auto compression_bound = ZSTD_compressBound(input_data.size_bytes());

  ByteStream compressed_data {};
  compressed_data.resize(compression_bound);

  const auto written_byte_count = ZSTD_compress(compressed_data.data(),
                                                compressed_data.size(),
                                                input_data.data(),
                                                input_data.size_bytes(),
                                                ZSTD_CLEVEL_DEFAULT);

  if (ZSTD_isError(written_byte_count)) {
    TACTILE_LOG_ERROR("Compression failed: {}",
                      ZSTD_getErrorName(written_byte_count));
    return unexpected(make_error(CompressionError::kCouldNotCompress));
  }

  compressed_data.resize(written_byte_count);
  compressed_data.shrink_to_fit();

  return compressed_data;
}

auto ZstdCompressionProvider::decompress(const ByteSpan input_data) const
    -> Result<ByteStream>
{
  const SetLogScope log_scope {"ZstdCompressionProvider"};

  const UniqueDStream stream {ZSTD_createDStream()};
  if (!stream) {
    TACTILE_LOG_ERROR("Could not create stream");
    return unexpected(make_error(CompressionError::kBadInit));
  }

  const auto init_stream_result = ZSTD_initDStream(stream.get());
  if (ZSTD_isError(init_stream_result)) {
    TACTILE_LOG_ERROR("Could not initialize stream: {}",
                      ZSTD_getErrorName(init_stream_result));
    return unexpected(make_error(CompressionError::kBadInit));
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
      TACTILE_LOG_ERROR("Decompression failed: {}",
                        ZSTD_getErrorName(decompress_result));
      return unexpected(make_error(CompressionError::kCouldNotDecompress));
    }

    byte_write_count += output_view.pos;
  } while (input_view.pos < input_view.size);

  flush_staging_buffer();

  decompressed_data.resize(byte_write_count);
  decompressed_data.shrink_to_fit();

  return decompressed_data;
}

}  // namespace tactile
