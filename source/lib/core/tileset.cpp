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

#include "tileset.hpp"

#include "common/debug/panic.hpp"

namespace tactile {

auto Tileset::index_of(const TilePos pos) const -> TileIndex
{
  if (contains(pos)) {
    return pos.row() * column_count + pos.col();
  }
  else {
    throw TactileError {"Invalid tile position!"};
  }
}

auto Tileset::contains(const TilePos pos) const -> bool
{
  const auto row = pos.row();
  const auto col = pos.col();
  return row >= 0 && col >= 0 && row < row_count && col < column_count;
}

auto AttachedTileset::is_valid_tile(const TileID tile_id) const -> bool
{
  return tile_id >= first_tile && tile_id <= last_tile;
}

auto AttachedTileset::is_single_tile_selected() const -> bool
{
  if (selection.has_value()) {
    return (selection->end - selection->begin) == TilePos {1, 1};
  }

  return false;
}

}  // namespace tactile
