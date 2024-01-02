// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/io/compression/compression_provider.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Provides compression and decompression using the Zstandard algorithm.
 *
 * \see https://github.com/facebook/zstd
 */
class TACTILE_FOUNDATION_API ZstdCompressionProvider final : public ICompressionProvider {
 public:
  [[nodiscard]]
  auto compress(ByteSpan data) const -> Result<ByteStream> override;

  [[nodiscard]]
  auto decompress(ByteSpan data) const -> Result<ByteStream> override;
};

}  // namespace tactile
