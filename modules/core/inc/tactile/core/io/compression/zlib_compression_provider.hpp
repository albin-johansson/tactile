// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/io/compression/compression_provider.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Provides compression and decompression using the Zlib library.
 *
 * \see https://github.com/madler/zlib
 */
class TACTILE_CORE_API ZlibCompressionProvider final : public ICompressionProvider {
 public:
  [[nodiscard]]
  auto compress(ByteSpan data) const -> Result<ByteStream> override;

  [[nodiscard]]
  auto decompress(ByteSpan data) const -> Result<ByteStream> override;

  /**
   * \brief Sets the compression level.
   *
   * \details Larger compression level values lead to higher quality compression, at the
   *          expense of worse execution speed. An empty value indicates that the default
   *          level should be used instead.
   *
   * \param level the new compression level. If set, clamped to
   *              [`min_compression_level()`, `max_compression_level()`].
   */
  void set_compression_level(Maybe<int> level);

  /**
   * \brief Returns the current compression level.
   *
   * \return the compression level; or nothing if the default level is being used.
   */
  [[nodiscard]]
  auto get_compression_level() const -> Maybe<int>;

  /**
   * \brief Returns the minimum (best speed) compression level.
   *
   * \return the minimum compression level.
   */
  [[nodiscard]]
  static auto min_compression_level() -> int;

  /**
   * \brief Returns the maximum (best quality) compression level.
   *
   * \return the maximum compression level.
   */
  [[nodiscard]]
  static auto max_compression_level() -> int;

 private:
  Maybe<int> mLevel;
};

}  // namespace tactile
