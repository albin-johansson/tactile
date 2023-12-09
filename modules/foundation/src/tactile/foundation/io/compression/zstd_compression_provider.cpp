// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/compression/zstd_compression_provider.hpp"

#include <algorithm>  // copy_n

#include <zstd.h>

#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/io/compression/compression_error.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile {
namespace {

struct DStreamDeleter final {
  void operator()(ZSTD_DStream* stream) noexcept { ZSTD_freeDStream(stream); }
};

using UniqueDStream = Unique<ZSTD_DStream, DStreamDeleter>;

}  // namespace

auto ZstdCompressionProvider::compress(const ByteSpan data) const -> Result<ByteStream>
{
  if (data.empty()) {
    return unexpected(make_compression_error(CompressionError::kNoData));
  }

  const auto compression_bound = ZSTD_compressBound(data.size_bytes());

  ByteStream byte_stream;
  byte_stream.resize(compression_bound);

  const auto written_byte_count = ZSTD_compress(byte_stream.data(),
                                                compression_bound,
                                                data.data(),
                                                data.size_bytes(),
                                                ZSTD_CLEVEL_DEFAULT);

  if (ZSTD_isError(written_byte_count)) {
    TACTILE_LOG_ERROR("[ZstdCompressionProvider] Compression failed: {}",
                      ZSTD_getErrorName(written_byte_count));
    return unexpected(make_compression_error(CompressionError::kInternalError));
  }

  byte_stream.resize(written_byte_count);
  byte_stream.shrink_to_fit();

  return byte_stream;
}

auto ZstdCompressionProvider::decompress(const ByteSpan data) const -> Result<ByteStream>
{
  if (data.empty()) {
    return unexpected(make_compression_error(CompressionError::kNoData));
  }

  const UniqueDStream stream {ZSTD_createDStream()};

  if (!stream) {
    TACTILE_LOG_ERROR("Could not create Zstd decompression stream");
    return unexpected(make_compression_error(CompressionError::kInternalError));
  }

  const auto init_result = ZSTD_initDStream(stream.get());
  if (ZSTD_isError(init_result)) {
    TACTILE_LOG_ERROR("Could not initialize Zstd decompression stream");
    return unexpected(make_compression_error(CompressionError::kInternalError));
  }

  const auto staging_buffer_size = ZSTD_DStreamOutSize();

  ByteStream staging_buffer;
  staging_buffer.resize(staging_buffer_size);

  ByteStream byte_stream;
  byte_stream.reserve(staging_buffer_size);  // Won't be enough, but better than nothing.

  ZSTD_inBuffer input_view {data.data(), data.size_bytes(), 0};
  ZSTD_outBuffer output_view {staging_buffer.data(), staging_buffer_size, 0};

  // Copies the contents of the staging buffer to the result byte stream.
  const auto flush_staging_buffer = [&] {
    std::copy_n(staging_buffer.data(), output_view.pos, std::back_inserter(byte_stream));
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
      TACTILE_LOG_ERROR("[ZstdCompressionProvider] Decompression failed: {}",
                        ZSTD_getErrorName(decompress_result));
      return unexpected(make_compression_error(CompressionError::kInternalError));
    }

    byte_write_count += output_view.pos;
  } while (input_view.pos < input_view.size);

  flush_staging_buffer();

  byte_stream.resize(byte_write_count);
  byte_stream.shrink_to_fit();

  return byte_stream;
}

}  // namespace tactile
