// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/compression/compression.hpp"

#include "tactile/foundation/io/compression/compression_error.hpp"
#include "tactile/foundation/io/compression/zlib_compression_provider.hpp"
#include "tactile/foundation/io/compression/zstd_compression_provider.hpp"

namespace tactile {

auto compress_with(const CompressionMode mode, const ByteSpan data) -> Result<ByteStream>
{
  switch (mode) {
    case CompressionMode::kNone: break;
    case CompressionMode::kZlib: return ZlibCompressionProvider {}.compress(data);
    case CompressionMode::kZstd: return ZstdCompressionProvider {}.compress(data);
  }

  return unexpected(make_compression_error(CompressionError::kInvalidMode));
}

auto decompress_with(const CompressionMode mode, const ByteSpan data)
    -> Result<ByteStream>
{
  switch (mode) {
    case CompressionMode::kNone: break;
    case CompressionMode::kZlib: return ZlibCompressionProvider {}.decompress(data);
    case CompressionMode::kZstd: return ZstdCompressionProvider {}.decompress(data);
  }

  return unexpected(make_compression_error(CompressionError::kInvalidMode));
}

}  // namespace tactile