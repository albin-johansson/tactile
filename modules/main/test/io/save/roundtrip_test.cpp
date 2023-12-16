// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <utility>  // move

#include <fmt/format.h>
#include <gtest/gtest.h>

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/io/filesystem.hpp"
#include "tactile/foundation/io/save/save_format_context.hpp"
#include "tactile/testutil/ir/ir_comparison.hpp"
#include "tactile/testutil/ir/layers.hpp"
#include "tactile/testutil/ir/maps.hpp"
#include "tactile/testutil/ir/objects.hpp"
#include "tactile/testutil/ir/properties.hpp"
#include "tactile/testutil/ir/tilesets.hpp"

using namespace tactile;
using namespace tactile::fs_literals;

class RoundtripTest : public testing::TestWithParam<String> {};

// NOLINTBEGIN(*-trailing-return-type)

// NOLINTBEGIN(*-use-anonymous-namespace)
INSTANTIATE_TEST_SUITE_P(Roundtrip,
                         RoundtripTest,
                         testing::Values(".yml", ".tmj", ".tmx"));
// NOLINTEND(*-use-anonymous-namespace)

// Tries to save and restore a map that uses a common subset of the save format features.
TEST_P(RoundtripTest, SaveAndLoadMap)
{
  const String& file_extension = GetParam();

  const auto map_directory = "assets/test/integration/roundtrip"_path;
  const auto map_filename = fmt::format("map{}", file_extension);
  const auto map_path = map_directory / map_filename;

  const MatrixExtent map_extent {4, 5};

  auto map = testutil::make_ir_map(map_filename, map_extent);
  map.meta.properties = testutil::make_basic_properties();

  auto tileset1 = testutil::make_dummy_ir_tileset("tileset-1");
  tileset1.meta.properties = testutil::make_basic_properties();

  auto tileset1_tile3 = testutil::make_ir_tile(TileIndex {3});
  tileset1_tile3.animation = {
    ir::AnimationFrame {TileIndex {3}, Milliseconds {50}},
    ir::AnimationFrame {TileIndex {4}, Milliseconds {423}},
    ir::AnimationFrame {TileIndex {10}, Milliseconds {65}},
  };

  auto tileset1_tile17 = testutil::make_ir_tile(TileIndex {17});
  tileset1_tile17.meta.properties = testutil::make_basic_properties();

  auto tileset1_tile42 = testutil::make_ir_tile(TileIndex {42});
  tileset1_tile42.objects = {
    testutil::make_ir_object(tactile::ObjectType::kPoint, map.next_object_id++),
    testutil::make_ir_object(tactile::ObjectType::kRect, map.next_object_id++),
    testutil::make_ir_object(tactile::ObjectType::kEllipse, map.next_object_id++)};

  tileset1.tiles = {std::move(tileset1_tile3),
                    std::move(tileset1_tile17),
                    std::move(tileset1_tile42)};

  auto tileset2 = testutil::make_dummy_ir_tileset("tileset-2");
  tileset2.meta.properties = testutil::make_basic_properties();

  auto tile_layer = testutil::make_ir_tile_layer(map.next_layer_id++, map_extent);
  tile_layer.meta.properties = testutil::make_basic_properties();
  tile_layer.tiles = testutil::make_tile_matrix_with_increasing_tiles(map_extent);
  tile_layer.opacity = 0.5f;

  auto point_object = testutil::make_ir_object(ObjectType::kPoint, map.next_object_id++);
  point_object.meta.properties = testutil::make_basic_properties();
  point_object.x = 832;
  point_object.y = -32;

  auto rect_object = testutil::make_ir_object(ObjectType::kRect, map.next_object_id++);
  rect_object.meta.properties = testutil::make_basic_properties();
  rect_object.x = 1963;
  rect_object.y = 0;
  rect_object.width = 34;
  rect_object.height = 93;

  auto ellipse_object =
      testutil::make_ir_object(ObjectType::kEllipse, map.next_object_id++);
  ellipse_object.meta.properties = testutil::make_basic_properties();
  ellipse_object.width = 50;
  ellipse_object.height = 10;
  ellipse_object.visible = false;

  auto object_layer = testutil::make_ir_object_layer(map.next_layer_id++);
  object_layer.meta.properties = testutil::make_basic_properties();
  object_layer.visible = false;
  object_layer.objects = {std::move(point_object),
                          std::move(rect_object),
                          std::move(ellipse_object)};

  auto group_layer = testutil::make_ir_group_layer(map.next_layer_id++);
  group_layer.meta.properties = testutil::make_basic_properties();
  group_layer.layers.push_back(std::move(object_layer));

  map.tilesets = {
    ir::TilesetRef {std::move(tileset1), TileID {1}},
    ir::TilesetRef {std::move(tileset2), TileID {1'000}},
  };

  map.layers.push_back(std::move(group_layer));
  map.layers.push_back(std::move(tile_layer));

  const SaveFormatWriteOptions write_options = {
    .base_dir = map_path.parent_path(),
    .use_external_tilesets = false,
    .use_indentation = true,
    .fold_tile_layer_data = false,
  };

  const SaveFormatReadOptions read_options = {
    .base_dir = map_path.parent_path(),
    .strict_mode = false,
  };

  const auto& save_format_context = SaveFormatContext::get();
  ASSERT_TRUE(save_format_context.save_map(map_path, map, write_options).has_value());

  const auto parsed_map = save_format_context.load_map(map_path, read_options);
  ASSERT_TRUE(parsed_map.has_value());

  testutil::expect_eq(*parsed_map, map);
}

// NOLINTEND(*-trailing-return-type)
