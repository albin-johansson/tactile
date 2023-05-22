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

#include <utility>  // pair, move

#include <doctest/doctest.h>

#include "common/primitives.hpp"
#include "common/tile_matrix.hpp"
#include "io/export/tactile_yaml_exporter.hpp"
#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/parse_map.hpp"

namespace tactile::test {
namespace {

inline const TileMatrix kTestTiles {
    {123, 8324, 4123, 68, 0},
    {7483, 32, 12944, 123, 8},
    {8, 284, 221, 435, 2123},
    {939, 842, 993, 759, 435},
    {57, 348, 118, 12, 136},
};

[[nodiscard]] auto _create_test_map() -> MapIR
{
  MapIR map;
  map.extent.rows = 5;
  map.extent.cols = 5;
  map.tile_size = {32, 32};

  map.next_layer_id = 1;
  map.next_object_id = 1;

  auto& layer = map.layers.emplace_back();
  layer.name = "Foo";
  layer.id = map.next_layer_id++;
  layer.type = LayerType::TileLayer;
  layer.opacity = 1.0f;
  layer.visible = true;

  auto& tile_layer = layer.data.emplace<TileLayerIR>();
  tile_layer.extent = map.extent;
  tile_layer.tiles = kTestTiles;

  return map;
}

using TileFormatFactory = TileFormatIR (*)();
using TestData = std::pair<std::string_view, TileFormatFactory>;

[[nodiscard]] auto _create_zlib_tile_format() -> TileFormatIR
{
  return {
      .encoding = TileEncoding::Base64,
      .compression = TileCompression::Zlib,
      .zlib_compression_level = 6,
  };
}

[[nodiscard]] auto _create_zstd_tile_format() -> TileFormatIR
{
  return {
      .encoding = TileEncoding::Base64,
      .compression = TileCompression::Zstd,
      .zstd_compression_level = 19,
  };
}

void _create_and_validate_yaml_map(const char* path, TileFormatFactory format_factory)
{
  {
    auto test_map = _create_test_map();
    test_map.tile_format = format_factory();

    Settings settings;
    save_map_as_tactile_yaml(path, test_map, settings);
  }

  const auto result = parse_map(path);
  REQUIRE(result.has_value());

  const auto& map = result.value();
  const auto& layer = map.layers.front();
  const auto& tile_layer = layer.as_tile_layer();

  REQUIRE(kTestTiles == tile_layer.tiles);
}

}  // namespace

TEST_SUITE("Tactile YAML format tile compression")
{
  TEST_CASE("Zlib")
  {
    _create_and_validate_yaml_map("zlib_compression_test.yaml", _create_zlib_tile_format);
  }

  TEST_CASE("Zstd")
  {
    _create_and_validate_yaml_map("zstd_compression_test.yaml", _create_zstd_tile_format);
  }
}

}  // namespace tactile::test
