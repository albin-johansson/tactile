// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/io/compress/compression_provider.hpp"

namespace tactile {

/**
 * Provides compression and decompression using the Zstandard algorithm.
 *
 * \see https://github.com/facebook/zstd
 */
class ZstdCompressionProvider final : public ICompressionProvider {
 public:
  [[nodiscard]]
  auto compress(ByteSpan input_data) const -> Result<ByteStream> override;

  [[nodiscard]]
  auto decompress(ByteSpan input_data) const -> Result<ByteStream> override;
};

}  // namespace tactile
