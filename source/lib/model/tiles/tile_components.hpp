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

#include <chrono>  // milliseconds

#include "tactile/core/containers/vector.hpp"
#include "core/ecs/entity.hpp"
#include "core/enums/tile_compression.hpp"
#include "core/enums/tile_encoding.hpp"
#include "core/math/vector.hpp"
#include "core/prelude.hpp"

namespace tactile {

/// Component that provides metadata about a tile in a tileset.
struct Tile final {
  TileIndex index;         ///< Index of the tile in the parent tileset.
  Int4 source {};          ///< Region in the parent tileset associated with the tile.
  Vector<Entity> objects;  ///< Objects embedded in the tile.
};

/// Represents a single frame in a tile animation.
struct TileAnimationFrame final {
  TileIndex tile_index {};                ///< Index of the shown tile during the frame.
  std::chrono::milliseconds duration {};  ///< The duration that the frame is shown.
};

/// Component that describes an animation associated with a tile.
struct TileAnimation final {
  Vector<TileAnimationFrame> frames;  ///< The associated animation frames.
  usize index {};                     ///< Current frame index.
  std::chrono::system_clock::time_point last_update {};  ///< The last frame update.
};

/// Component describing the tile layer data format utilized by a map.
/// TODO move to map components
struct TileFormat final {
  TileEncoding encoding {TileEncoding::Plain};          ///< Encoding strategy.
  TileCompression compression {TileCompression::None};  ///< Compression strategy.
  int zlib_compression_level {-1};  ///< Zlib compression level (if Zlib is used).
  int zstd_compression_level {3};   ///< Zstd compression level (if Zstd is used).
};

}  // namespace tactile
