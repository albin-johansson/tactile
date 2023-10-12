// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/compression/compression.hpp"

#include "tactile/core/io/compression/compression_error.hpp"
#include "tactile/core/io/compression/zlib_compression_provider.hpp"
#include "tactile/core/io/compression/zstd_compression_provider.hpp"

namespace tactile {

auto compress_with(const CompressionMode mode, const ByteSpan data) -> Result<ByteStream>
{
  switch (mode) {
    case CompressionMode::kNone: break;
    case CompressionMode::kZlib: return ZlibCompressionProvider {}.compress(data);
    case CompressionMode::kZstd: return ZstdCompressionProvider {}.compress(data);
  }

  return error(CompressionError::kInvalidMode);
}

auto decompress_with(const CompressionMode mode, const ByteSpan data)
    -> Result<ByteStream>
{
  switch (mode) {
    case CompressionMode::kNone: break;
    case CompressionMode::kZlib: return ZlibCompressionProvider {}.decompress(data);
    case CompressionMode::kZstd: return ZstdCompressionProvider {}.decompress(data);
  }

  return error(CompressionError::kInvalidMode);
}

}  // namespace tactile