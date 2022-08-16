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

#include <gtest/gtest.h>

#include "core/common/tiles.hpp"
#include "io/map/parse/parse_map.hpp"

namespace tactile::test {

TEST(CompressedTileData, TactileYAMLWithZlib)
{
  const auto result = io::parse_map("test-resources/yaml/compressed_zlib.yaml");
  ASSERT_EQ(io::ParseError::None, result.error());

  const auto& map = result.data();
  ASSERT_EQ(1, map.layers.size());

  const auto& layer = map.layers.front();
  ASSERT_EQ(LayerType::TileLayer, layer.type);

  const auto& tile_layer = std::get<ir::TileLayerData>(layer.data);

  const TileMatrix expected {{1000, 2000, 3000}, {4000, 5000, 6000}, {7000, 8000, 9000}};
  ASSERT_EQ(expected, tile_layer.tiles);
}

}  // namespace tactile::test
