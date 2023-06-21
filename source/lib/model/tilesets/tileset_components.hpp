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

#include "common/primitives.hpp"
#include "common/region.hpp"
#include "common/tile_pos.hpp"
#include "common/type/ecs.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/math.hpp"
#include "common/type/maybe.hpp"
#include "common/type/vector.hpp"
#include "model/components/viewport.hpp"

namespace tactile {

struct TilesetRenderCache final {
  mutable HashMap<TileIndex, TileIndex> appearance;
};

/// Represents a collection of tiles encoded in a texture.
struct Tileset final {
  Entity texture;                             ///< The associated texture entity.
  Int2 tile_size {};                          ///< The size of each tile.
  int32 row_count {};                         ///< The amount of tile rows.
  int32 column_count {};                      ///< The amount tile columns.
  Float2 uv_size {};                          ///< Size of a tile in texture coordinates.
  HashMap<TileIndex, Entity> tile_index_map;  ///< Maps local indices to tile entities.
  Vector<Entity> tiles;                       ///< Tile definitions.
  Maybe<TileIndex> selected_tile_index;       ///< Local index of selected tile.
};

struct AttachedTileset final {
  Entity tileset {kNullEntity};  ///< The referenced tileset.
  TileID first_tile {};          ///< The first associated tile ID (inclusive).
  TileID last_tile {};           ///< The last associated tile ID (inclusive).
  Maybe<Region> selection;       ///< Selected region.
  bool embedded {};              ///< Whether the tileset is embedded in the map.
};

}  // namespace tactile
