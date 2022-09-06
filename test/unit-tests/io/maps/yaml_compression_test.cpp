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

#include <utility>  // pair, move

#include <gtest/gtest.h>

#include "core/common/fs.hpp"
#include "core/common/tiles.hpp"
#include "core/util/tiles.hpp"
#include "io/map/emit/emit_info.hpp"
#include "io/map/emit/emitter.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/parse_map.hpp"

namespace tactile::test {
namespace {

inline const TileMatrix test_tiles {
    {123, 8324, 4123, 68, 0},
    {7483, 32, 12944, 123, 8},
    {8, 284, 221, 435, 2123},
    {939, 842, 993, 759, 435},
    {57, 348, 118, 12, 136},
};

[[nodiscard]] auto create_test_map() -> ir::MapData
{
  ir::MapData map;
  map.row_count = 5;
  map.col_count = 5;
  map.tile_size = {32, 32};

  map.next_layer_id = 1;
  map.next_object_id = 1;

  auto& layer = map.layers.emplace_back();
  layer.name = "Foo";
  layer.id = map.next_layer_id++;
  layer.type = LayerType::TileLayer;
  layer.opacity = 1.0f;
  layer.visible = true;

  auto& tile_layer = layer.data.emplace<ir::TileLayerData>();
  tile_layer.row_count = map.row_count;
  tile_layer.col_count = map.col_count;
  tile_layer.tiles = test_tiles;

  return map;
}

using TileFormatFactory = ir::TileFormatData (*)();
using TestData = std::pair<std::string_view, TileFormatFactory>;

[[nodiscard]] auto create_zlib_tile_format() -> ir::TileFormatData
{
  return {
      .encoding = TileEncoding::Base64,
      .compression = TileCompression::Zlib,
      .zlib_compression_level = 6,
  };
}

[[nodiscard]] auto create_zstd_tile_format() -> ir::TileFormatData
{
  return {
      .encoding = TileEncoding::Base64,
      .compression = TileCompression::Zstd,
      .zstd_compression_level = 19,
  };
}

const auto cases =
    testing::Values(TestData {"zlib_compression_test.yaml", create_zlib_tile_format},
                    TestData {"zstd_compression_test.yaml", create_zstd_tile_format});

}  // namespace

struct CompressedYamlMapTest : testing::TestWithParam<TestData>
{};

TEST_P(CompressedYamlMapTest, EmitAndParseMap)
{
  const auto [path, format_factory] = GetParam();

  {
    auto test_map = create_test_map();
    test_map.tile_format = format_factory();

    const io::EmitInfo info {path, std::move(test_map)};
    io::emit_yaml_map(info);
  }

  const auto result = io::parse_map(path);
  ASSERT_EQ(io::ParseError::None, result.error());

  const auto& map = result.data();
  const auto& layer = map.layers.front();
  const auto& tile_layer = layer.as_tile_layer();

  ASSERT_EQ(test_tiles, tile_layer.tiles);
}

INSTANTIATE_TEST_SUITE_P(CompressedYamlMapTests, CompressedYamlMapTest, cases);

}  // namespace tactile::test
