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

#include "tileset_ops.hpp"

#include "common/util/lookup.hpp"
#include "tactile/core/common/error.hpp"

namespace tactile::sys {

auto tile_count(const Tileset& tileset) -> int32
{
  return tileset.row_count * tileset.column_count;
}

auto tile_index_at(const Tileset& tileset, const TilePos pos) -> TileIndex
{
  if (is_valid_tile(tileset, pos)) {
    return pos.row() * tileset.column_count + pos.col();
  }
  else {
    throw Error {"Invalid tile position"};
  }
}

auto is_valid_tile(const Tileset& tileset, const TilePos pos) -> bool
{
  const auto row = pos.row();
  const auto col = pos.col();
  return row >= 0 && col >= 0 && row < tileset.row_count && col < tileset.column_count;
}

auto get_active_tile(const Tileset& tileset) -> Entity
{
  if (tileset.selected_tile_index.has_value()) {
    return lookup_in(tileset.tile_index_map, *tileset.selected_tile_index);
  }

  return kNullEntity;
}

}  // namespace tactile::sys
