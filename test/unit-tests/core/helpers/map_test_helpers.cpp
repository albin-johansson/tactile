// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "map_test_helpers.hpp"

#include <gtest/gtest.h>

#include "core/map.hpp"
#include "core/tile_pos.hpp"
#include "core/util/functional.hpp"

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
