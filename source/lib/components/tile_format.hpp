/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/enum/tile_compression.hpp"
#include "common/enum/tile_encoding.hpp"

namespace tactile {

/**
 * Component describing the tile layer data format utilized by a map.
 *
 * \see https://github.com/facebook/zstd/issues/3133 for Zstd compression levels.
 */
struct TileFormat final {
  TileEncoding encoding {TileEncoding::Plain};          ///< Encoding strategy.
  TileCompression compression {TileCompression::None};  ///< Compression strategy.
  int zlib_compression_level {-1};  ///< Zlib compression level (if Zlib is used).
  int zstd_compression_level {3};   ///< Zstd compression level (if Zstd is used).

  /// Indicates whether the currently described format supports compression.
  [[nodiscard]] auto supports_compression() const -> bool;
};

/**
 * Indicates whether the specified value is a valid Zlib compression level.
 *
 * \param level the value to evaluate.
 * \return true if the value is valid; false otherwise.
 */
[[nodiscard]] auto is_valid_zlib_compression_level(int level) -> bool;

/**
 * Indicates whether the specified value is a valid Zstd compression level.
 *
 * \param level the value to evaluate.
 * \return true if the value is valid; false otherwise.
 */
[[nodiscard]] auto is_valid_zstd_compression_level(int level) -> bool;

[[nodiscard]] auto min_zlib_compression_level() -> int;
[[nodiscard]] auto max_zlib_compression_level() -> int;

[[nodiscard]] auto min_zstd_compression_level() -> int;
[[nodiscard]] auto max_zstd_compression_level() -> int;

}  // namespace tactile
