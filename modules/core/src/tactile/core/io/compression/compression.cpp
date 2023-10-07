// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/compression/compression.hpp"

#include "tactile/core/io/compression/compression_error.hpp"
#include "tactile/core/io/compression/zlib_compressor.hpp"
#include "tactile/core/io/compression/zstd_compressor.hpp"

namespace tactile {

auto compress_with(const CompressionMode mode, const ByteSpan data) -> Result<ByteStream>
{
  switch (mode) {
    case CompressionMode::kNone: break;
    case CompressionMode::kZlib: return ZlibCompressor {}.compress(data);
    case CompressionMode::kZstd: return ZstdCompressor {}.compress(data);
  }

  return unexpected(compression_error(CompressionError::kInvalidMode));
}

auto decompress_with(const CompressionMode mode, const ByteSpan data) -> Result<ByteStream>
{
  switch (mode) {
    case CompressionMode::kNone: break;
    case CompressionMode::kZlib: return ZlibCompressor {}.decompress(data);
    case CompressionMode::kZstd: return ZstdCompressor {}.decompress(data);
  }

  return unexpected(compression_error(CompressionError::kInvalidMode));
}

}  // namespace tactile