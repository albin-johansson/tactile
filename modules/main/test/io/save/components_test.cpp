// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/testutil/ir/components.hpp"

#include <gtest/gtest.h>

#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/io/save/save_format_context.hpp"
#include "tactile/testutil/ir/ir_comparison.hpp"
#include "tactile/testutil/ir/layers.hpp"
#include "tactile/testutil/ir/maps.hpp"
#include "tactile/testutil/ir/objects.hpp"
#include "tactile/testutil/ir/tilesets.hpp"

using namespace tactile;

class ComponentsTest : public testing::Test {};

// NOLINTBEGIN(*-trailing-return-type)

TEST_F(ComponentsTest, SaveAndLoadMapWithComponents)
{
  const FilePath base_directory {"assets/test/integration/components"};
  const String map_filename {"components.yml"};
  const auto map_path = base_directory / map_filename;

  const MatrixExtent map_extent {15, 15};
  auto map = testutil::make_ir_map(map_filename, map_extent);

  const auto empty_component = testutil::make_empty_ir_component("C1");
  const auto basic_component = testutil::make_ir_component_with_basic_properties("C2");
  const auto full_component = testutil::make_ir_component_with_all_properties("C3");

  const auto empty_attached_component =
      testutil::make_ir_attached_component(empty_component);
  const auto basic_attached_component =
      testutil::make_ir_attached_component(basic_component);
  const auto full_attached_component =
      testutil::make_ir_attached_component(full_component);

  auto tile = testutil::make_ir_tile(TileIndex {8});
  tile.meta.components.push_back(basic_attached_component);

  auto tileset = testutil::make_dummy_ir_tileset("TS");
  tileset.meta.components.push_back(basic_attached_component);
  tileset.tiles.push_back(tile);

  auto object = testutil::make_ir_object(ObjectType::kRect, map.next_object_id++);
  object.meta.components.push_back(full_attached_component);

  auto layer = testutil::make_ir_object_layer(map.next_layer_id++);
  layer.meta.components.push_back(basic_attached_component);
  layer.objects.push_back(object);

  map.components = {empty_component, basic_component, full_component};
  map.meta.components.push_back(empty_attached_component);
  map.meta.components.push_back(basic_attached_component);
  map.meta.components.push_back(full_attached_component);
  map.tilesets.push_back(ir::TilesetRef {tileset, TileID {1}});
  map.layers.push_back(layer);

  const auto& save_format_context = SaveFormatContext::get();

  const SaveFormatWriteOptions write_options = {
    .base_dir = base_directory,
    .use_external_tilesets = false,
    .use_indentation = true,
    .fold_tile_layer_data = true,
  };
  ASSERT_TRUE(save_format_context
                  .save_map(SaveFormatId::kTactileYaml, map_path, map, write_options)
                  .has_value());

  const SaveFormatReadOptions read_options = {
    .base_dir = base_directory,
    .strict_mode = false,
  };
  const auto parsed_map = save_format_context.load_map(map_path, read_options);
  ASSERT_TRUE(parsed_map.has_value());

  testutil::expect_eq(*parsed_map, map);
}

// NOLINTEND(*-trailing-return-type)
