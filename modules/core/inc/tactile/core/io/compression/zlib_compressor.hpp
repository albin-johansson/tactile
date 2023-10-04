// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/io/compression/compression_provider.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Provides compression and decompression using the Zlib library.
 */
class ZlibCompressor final : public ICompressionProvider {
 public:
  [[nodiscard]]
  TACTILE_CORE_API auto compress(Span<const uchar> data) const
      -> Result<ByteStream> override;

  [[nodiscard]]
  TACTILE_CORE_API auto decompress(Span<const uchar> data) const
      -> Result<ByteStream> override;

  TACTILE_CORE_API void set_compression_level(Maybe<int> level);

  [[nodiscard]]
  TACTILE_CORE_API auto get_compression_level() const -> Maybe<int>;

  [[nodiscard]]
  TACTILE_CORE_API static auto min_compression_level() -> int;

  [[nodiscard]]
  TACTILE_CORE_API static auto max_compression_level() -> int;

 private:
  Maybe<int> mLevel;
};

}  // namespace tactile
