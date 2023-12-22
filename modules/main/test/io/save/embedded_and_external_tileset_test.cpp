// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <filesystem>  // absolute
#include <ostream>     // ostream

#include <fmt/format.h>
#include <gtest/gtest.h>

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/io/filesystem.hpp"
#include "tactile/foundation/io/save/save_format_context.hpp"
#include "tactile/testutil/ir/ir_comparison.hpp"
#include "tactile/testutil/ir/maps.hpp"
#include "tactile/testutil/ir/tilesets.hpp"

using namespace tactile;

using fs_literals::operator""_path;

struct EmbeddedAndExternalTilesetTestData final {
  SaveFormatId save_format_id;
  String file_extension;
  bool embed_tilesets;
};

auto operator<<(std::ostream& stream, const EmbeddedAndExternalTilesetTestData& test_data)
    -> std::ostream&
{
  return stream << to_string(test_data.save_format_id) << " with "
                << (test_data.embed_tilesets ? "embedded" : "external") << " tilesets";
}

class EmbeddedAndExternalTilesetTest
  : public testing::TestWithParam<EmbeddedAndExternalTilesetTestData> {};

INSTANTIATE_TEST_SUITE_P(
    EmbeddedAndExternalTileset,
    EmbeddedAndExternalTilesetTest,
    testing::Values(
        EmbeddedAndExternalTilesetTestData {SaveFormatId::kTactileYaml, ".yml", true},
        EmbeddedAndExternalTilesetTestData {SaveFormatId::kTactileYaml, ".yml", false},
        EmbeddedAndExternalTilesetTestData {SaveFormatId::kTiledJson, ".tmj", true},
        EmbeddedAndExternalTilesetTestData {SaveFormatId::kTiledJson, ".tmj", false},
        EmbeddedAndExternalTilesetTestData {SaveFormatId::kTiledXml, ".tmx", true},
        EmbeddedAndExternalTilesetTestData {SaveFormatId::kTiledXml, ".tmx", false}));

TEST_P(EmbeddedAndExternalTilesetTest, SaveAndLoadMap)
{
  const auto& test_data = GetParam();

  const auto map_directory = "assets/test/integration/tilesets"_path;
  const auto map_filename =
      fmt::format("map_with_{}_tilesets{}",
                  test_data.embed_tilesets ? "embedded" : "external",
                  test_data.file_extension);
  const auto map_path = map_directory / map_filename;

  auto map = testutil::make_ir_map(map_filename, MatrixExtent {1, 1});
  map.tilesets = {
    ir::TilesetRef {
      .tileset = testutil::make_dummy_ir_tileset("TS1"),
      .first_tile_id = TileID {1},
    },
    ir::TilesetRef {
      .tileset = testutil::make_dummy_ir_tileset("TS2"),
      .first_tile_id = TileID {10'000},
    },
  };

  const SaveFormatWriteOptions write_options = {
    .base_dir = map_directory,
    .use_external_tilesets = !test_data.embed_tilesets,
    .use_indentation = true,
    .fold_tile_layer_data = false,
  };

  const SaveFormatReadOptions read_options = {
    .base_dir = map_directory,
    .strict_mode = true,
  };

  const auto& save_format_context = SaveFormatContext::get();
  ASSERT_TRUE(
      save_format_context.save_map(test_data.save_format_id, map_path, map, write_options)
          .has_value());

  const auto parsed_map = save_format_context.load_map(map_path, read_options);
  ASSERT_TRUE(parsed_map.has_value());

  testutil::expect_eq(*parsed_map, map);
}
