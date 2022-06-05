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

#include <entt/entity/registry.hpp>

#include "core/components/layers.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "misc/panic.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _is_valid_position(const comp::TileLayer& layer,
                                      const usize row,
                                      const usize col) -> bool
{
  return row < layer.matrix.size() && col < layer.matrix[0].size();
}

}  // namespace

void set_tile(comp::TileLayer& layer, const TilePos& pos, const TileID tile)
{
  const auto row = pos.row_index();
  const auto col = pos.col_index();

  if (_is_valid_position(layer, row, col)) {
    layer.matrix[row][col] = tile;
  }
  else {
    throw TactileError{"Invalid tile layer position!"};
  }
}

void set_tiles(comp::TileLayer& layer, const TileCache& tiles)
{
  for (const auto& [position, tile] : tiles) {
    const auto row = position.row_index();
    const auto col = position.col_index();
    TACTILE_ASSERT(_is_valid_position(layer, row, col));
    layer.matrix[row][col] = tile;
  }
}

auto get_tile(const comp::TileLayer& layer, const TilePos& pos) -> TileID
{
  const auto row = pos.row_index();
  const auto col = pos.col_index();

  if (_is_valid_position(layer, row, col)) {
    return layer.matrix[row][col];
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

}  // namespace tactile::sys
