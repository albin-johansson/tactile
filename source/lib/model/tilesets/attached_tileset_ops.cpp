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

#include "attached_tileset_ops.hpp"

namespace tactile::sys {

auto is_valid_tile(const AttachedTileset& attached_tileset, const TileID tile_id) -> bool
{
  return tile_id >= attached_tileset.first_tile && tile_id <= attached_tileset.last_tile;
}

auto to_tile_index(const AttachedTileset& attached_tileset, const TileID tile_id)
    -> Maybe<TileIndex>
{
  if (is_valid_tile(attached_tileset, tile_id)) {
    return tile_id - attached_tileset.first_tile;
  }

  return kNone;
}

auto is_single_tile_selected(const AttachedTileset& attached_tileset) -> bool
{
  if (attached_tileset.selection.has_value()) {
    return (attached_tileset.selection->end - attached_tileset.selection->begin) ==
           TilePos {1, 1};
  }

  return false;
}

}  // namespace tactile::sys
