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

#include "layer_system.hpp"
#include "misc/throw.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto is_valid_position(const comp::TileLayer& layer,
                                     const usize row,
                                     const usize col) -> bool
{
  return row < layer.matrix.size() && col < layer.matrix[0].size();
}

}  // namespace

auto get_tile_layer_entity(const entt::registry& registry, const LayerID id)
    -> entt::entity
{
  const auto entity = sys::find_layer(registry, id);
  if (entity != entt::null && registry.all_of<comp::TileLayer>(entity)) {
    return entity;
  }
  else {
    throw_traced(TactileError{"Invalid tile layer identifier!"});
  }
}

void set_tile(comp::TileLayer& layer, const TilePos& position, const TileID tile)
{
  const auto row = position.row_index();
  const auto col = position.col_index();

  if (is_valid_position(layer, row, col)) {
    layer.matrix[row][col] = tile;
  }
  else {
    throw_traced(TactileError{"Invalid tile layer position!"});
  }
}

void set_tiles(comp::TileLayer& layer, const TileCache& tiles)
{
  for (const auto& [position, tile] : tiles) {
    const auto row = position.row_index();
    const auto col = position.col_index();
    TACTILE_ASSERT(is_valid_position(layer, row, col));
    layer.matrix[row][col] = tile;
  }
}

auto get_tile(const comp::TileLayer& layer, const TilePos& position) -> TileID
{
  const auto row = position.row_index();
  const auto col = position.col_index();

  if (is_valid_position(layer, row, col)) {
    return layer.matrix[row][col];
  }
  else {
    return empty_tile;
  }
}

}  // namespace tactile::sys
