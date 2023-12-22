// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <utility>  // move

#include <fmt/format.h>
#include <gtest/gtest.h>

#include "tactile/foundation/io/filesystem.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_context.hpp"
#include "tactile/testutil/ir/ir_comparison.hpp"
#include "tactile/testutil/ir/layers.hpp"
#include "tactile/testutil/ir/maps.hpp"

// NOLINTBEGIN(*-trailing-return-type)

using namespace tactile;

using fs_literals::operator""_path;

struct NestedLayersTestData final {
  SaveFormatId save_format_id;
  String file_extension;
};

class NestedLayersTest : public testing::TestWithParam<NestedLayersTestData> {};

// NOLINTBEGIN(*-use-anonymous-namespace)
INSTANTIATE_TEST_SUITE_P(
    NestedLayers,
    NestedLayersTest,
    testing::Values(NestedLayersTestData {SaveFormatId::kTactileYaml, ".yml"},
                    NestedLayersTestData {SaveFormatId::kTiledJson, ".tmj"},
                    NestedLayersTestData {SaveFormatId::kTiledXml, ".tmx"}));
// NOLINTEND(*-use-anonymous-namespace)

TEST_P(NestedLayersTest, SaveAndLoadMapWithNestedLayers)
{
  const auto& test_data = GetParam();

  const auto map_directory = "assets/test/integration/nested"_path;
  const auto map_filename = fmt::format("nested_layers{}", test_data.file_extension);
  const auto map_path = map_directory / map_filename;

  const MatrixExtent map_extent {6, 8};
  auto map = testutil::make_ir_map(map_filename, map_extent);

  map.layers = {
    testutil::make_ir_tile_layer(1, map_extent),
    testutil::make_ir_object_layer(2),
    testutil::make_complex_ir_group_layer(3, map_extent),
  };

  const SaveFormatWriteOptions write_options = {
    .base_dir = map_directory,
    .use_external_tilesets = false,
    .use_indentation = true,
    .fold_tile_layer_data = false,
  };

  const SaveFormatReadOptions read_options = {
    .base_dir = map_directory,
    .strict_mode = false,
  };

  const auto& save_format_context = SaveFormatContext::get();
  ASSERT_TRUE(
      save_format_context.save_map(test_data.save_format_id, map_path, map, write_options)
          .has_value());

  const auto parsed_map = save_format_context.load_map(map_path, read_options);
  ASSERT_TRUE(parsed_map.has_value());

  testutil::expect_eq(*parsed_map, map);
}

// NOLINTEND(*-trailing-return-type)
