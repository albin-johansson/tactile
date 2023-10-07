// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/io/compression/compression_provider.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Provides compression and decompression using the Zstandard algorithm.
 *
 * \see https://github.com/facebook/zstd
 */
class ZstdCompressionProvider final : public ICompressionProvider {
 public:
  [[nodiscard]]
  TACTILE_CORE_API auto compress(ByteSpan data) const -> Result<ByteStream> override;

  [[nodiscard]]
  TACTILE_CORE_API auto decompress(ByteSpan data) const -> Result<ByteStream> override;
};

}  // namespace tactile
