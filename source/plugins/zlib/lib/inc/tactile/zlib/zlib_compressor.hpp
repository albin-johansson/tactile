// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/io/compress/compressor.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/zlib/api.hpp"

namespace tactile {

/**
 * Provides compression using the Zlib library.
 *
 * \see https://github.com/madler/zlib
 */
class TACTILE_ZLIB_API ZlibCompressor final : public ICompressor
{
 public:
  [[nodiscard]]
  auto compress(ByteSpan input_data) const
      -> std::expected<ByteStream, std::error_code> override;

  [[nodiscard]]
  auto decompress(ByteSpan input_data) const
      -> std::expected<ByteStream, std::error_code> override;
};

}  // namespace tactile
