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

#include "layer.hpp"

#include <algorithm>  // find

#include "common/debug/assert.hpp"

namespace tactile {

void TileLayer::set_tile(const TilePos pos, const TileID tile_id)
{
  if (contains(pos)) [[likely]] {
    const auto row = static_cast<usize>(pos.row());
    const auto col = static_cast<usize>(pos.col());
    tiles[row][col] = tile_id;
  }
}

auto TileLayer::tile_at(const TilePos pos) const -> Maybe<TileID>
{
  if (contains(pos)) [[likely]] {
    const auto row = static_cast<usize>(pos.row());
    const auto col = static_cast<usize>(pos.col());
    return tiles[row][col];
  }
  else {
    return nothing;
  }
}

auto TileLayer::contains(const TilePos pos) const -> bool
{
  const auto row = pos.row();
  const auto col = pos.col();
  return row >= 0 &&                                //
         col >= 0 &&                                //
         static_cast<usize>(row) < tiles.size() &&  //
         static_cast<usize>(col) < tiles.at(0).size();
}

void GroupLayer::append(const Entity layer_entity)
{
  TACTILE_ASSERT(std::find(children.begin(), children.end(), layer_entity) ==
                 children.end());
  children.push_back(layer_entity);
}

}  // namespace tactile
