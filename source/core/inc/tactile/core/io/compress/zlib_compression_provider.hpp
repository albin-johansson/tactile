// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/io/compress/compression_provider.hpp"

namespace tactile {

/**
 * Provides compression using the Zlib library.
 *
 * \see https://github.com/madler/zlib
 */
class ZlibCompressionProvider final : public ICompressionProvider {
 public:
  [[nodiscard]]
  auto compress(ByteSpan input_data) const -> Result<ByteStream> override;

  [[nodiscard]]
  auto decompress(ByteSpan input_data) const -> Result<ByteStream> override;
};

}  // namespace tactile
