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

#include "tile_layer_ops.hpp"

#include "tactile/core/containers/queue.hpp"

namespace tactile::sys {

void flood_tiles(TileLayer& tile_layer,
                 const TilePos& origin,
                 const TileID replacement,
                 Vector<TilePos>* affected)
{
  const auto target = tile_at(tile_layer, origin);

  if (!is_valid_tile(tile_layer, origin) || target == replacement) {
    return;
  }

  Queue<TilePos> affected_positions;

  auto replace_tile = [&](const TilePos& position) {
    set_tile(tile_layer, position, replacement);

    if (affected != nullptr) {
      affected->push_back(position);
    }

    affected_positions.push(position);
  };

  replace_tile(origin);

  while (!affected_positions.empty()) {
    const auto affected_position = affected_positions.front();
    affected_positions.pop();

    auto check_position = [&](const TilePos& position) {
      if (tile_at(tile_layer, position) == target) {
        replace_tile(position);
      }
    };

    check_position(affected_position.west());
    check_position(affected_position.east());
    check_position(affected_position.south());
    check_position(affected_position.north());
  }
}

auto set_tile(TileLayer& tile_layer, const TilePos pos, const TileID tile_id) -> Result
{
  if (is_valid_tile(tile_layer, pos)) [[likely]] {
    const auto row = static_cast<usize>(pos.row());
    const auto col = static_cast<usize>(pos.col());
    tile_layer.tiles[row][col] = tile_id;

    return success;
  }

  return failure;
}

auto tile_at(const TileLayer& tile_layer, const TilePos pos) -> Maybe<TileID>
{
  if (is_valid_tile(tile_layer, pos)) [[likely]] {
    const auto row = static_cast<usize>(pos.row());
    const auto col = static_cast<usize>(pos.col());
    return tile_layer.tiles[row][col];
  }
  else {
    return {};
  }
}

auto is_valid_tile(const TileLayer& tile_layer, const TilePos pos) -> bool
{
  const auto row = pos.row();
  const auto col = pos.col();

  const auto row_count = tile_layer.tiles.size();
  const auto col_count = tile_layer.tiles.at(0).size();

  return row >= 0 &&                             //
         col >= 0 &&                             //
         static_cast<usize>(row) < row_count &&  //
         static_cast<usize>(col) < col_count;
}

}  // namespace tactile::sys
