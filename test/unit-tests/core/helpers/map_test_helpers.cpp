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

#include "map_test_helpers.hpp"

#include <gtest/gtest.h>

#include "core/common/functional.hpp"
#include "core/map.hpp"
#include "core/tile_pos.hpp"

namespace tactile::test {

void set_all_tiles(TileLayer& layer, const TileID tile)
{
  invoke_mn(layer.row_count(), layer.column_count(), [&](usize r, usize c) {
    layer.set_tile(TilePos::from(r, c), tile);
  });
}

void verify_all_tiles_matches(const TileLayer& layer, const TileID tile)
{
  invoke_mn(layer.row_count(), layer.column_count(), [&](usize r, usize c) {
    ASSERT_EQ(tile, layer.tile_at(TilePos::from(r, c)));
  });
}

auto add_tile_layer(Map& map) -> TileLayer&
{
  const auto layerId = map.add_tile_layer();
  return map.view_tile_layer(layerId);
}

}  // namespace tactile::test
