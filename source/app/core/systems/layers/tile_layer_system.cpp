/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "tile_layer_system.hpp"

#include <queue>  // queue

#include <entt/entity/registry.hpp>

#include "core/common/ecs.hpp"
#include "core/components/layers.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/map_system.hpp"
#include "core/tile_pos.hpp"
#include "misc/panic.hpp"

namespace tactile::sys {

void flood(comp::TileLayer& layer,
           const TilePos& origin,
           const TileID replacement,
           std::vector<TilePos>& affected)
{
  const auto target = get_tile(layer, origin);

  if (!is_valid_position(layer, origin) || (target == replacement)) {
    return;
  }

  std::queue<TilePos> positions;
  positions.push(origin);

  set_tile(layer, origin, replacement);
  affected.push_back(origin);

  auto update = [&](const TilePos& position) {
    if (is_valid_position(layer, position)) {
      const auto tile = get_tile(layer, position);
      if (tile == target) {
        set_tile(layer, position, replacement);
        affected.push_back(position);
        positions.push(position);
      }
    }
  };

  while (!positions.empty()) {
    const auto position = positions.front();
    positions.pop();

    update(position.west());
    update(position.east());
    update(position.south());
    update(position.north());
  }
}

void set_tile(comp::TileLayer& layer, const TilePos& pos, const TileID tile)
{
  if (is_valid_position(layer, pos)) {
    layer.matrix[pos.row_index()][pos.col_index()] = tile;
  }
  else {
    throw TactileError{"Invalid tile layer position!"};
  }
}

void set_tiles(comp::TileLayer& layer, const TileCache& tiles)
{
  for (const auto& [pos, tile] : tiles) {
    TACTILE_ASSERT(is_valid_position(layer, pos));
    layer.matrix[pos.row_index()][pos.col_index()] = tile;
  }
}

auto get_tile(const comp::TileLayer& layer, const TilePos& pos) -> TileID
{
  if (is_valid_position(layer, pos)) {
    return layer.matrix[pos.row_index()][pos.col_index()];
  }
  else {
    return empty_tile;
  }
}

void each_tile(const comp::TileLayer& layer,
               const std::function<void(usize, usize, TileID)>& callable)
{
  TACTILE_ASSERT(!layer.matrix.empty());
  TACTILE_ASSERT(!layer.matrix.at(0).empty());

  const auto nRows = layer.matrix.size();
  const auto nCols = layer.matrix.at(0).size();

  for (usize row = 0; row < nRows; ++row) {
    for (usize col = 0; col < nCols; ++col) {
      const auto id = layer.matrix[row][col];
      callable(row, col, id);
    }
  }
}

auto is_valid_position(const comp::TileLayer& layer, const TilePos& pos) -> bool
{
  return pos.row() >= 0 &&  //
         pos.col() >= 0 &&  //
         pos.row_index() < layer.matrix.size() &&
         pos.col_index() < layer.matrix.at(0).size();
}

}  // namespace tactile::sys
