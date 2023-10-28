// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <filesystem>  // absolute
#include <ostream>     // ostream

#include <fmt/format.h>
#include <gtest/gtest.h>

#include "tactile/core/container/string.hpp"
#include "tactile/core/io/save/save_format_manager.hpp"
#include "test/ir_comparison.hpp"

using namespace tactile;
using namespace tactile::fs_literals;

struct EmbeddedAndExternalTilesetTestData final {
  String file_extension;
  bool embed_tilesets;
};

auto operator<<(std::ostream& stream, const EmbeddedAndExternalTilesetTestData& test_data)
    -> std::ostream&
{
  stream << test_data.file_extension << " with "
         << (test_data.embed_tilesets ? "embedded" : "external") << " tilesets";
  return stream;
}

class EmbeddedAndExternalTilesetTest
  : public testing::TestWithParam<EmbeddedAndExternalTilesetTestData> {};

INSTANTIATE_TEST_SUITE_P(
    EmbeddedAndExternalTileset,
    EmbeddedAndExternalTilesetTest,
    testing::Values(EmbeddedAndExternalTilesetTestData {".tmj", true},
                    EmbeddedAndExternalTilesetTestData {".tmj", false}));

TEST_P(EmbeddedAndExternalTilesetTest, SaveAndLoadMap)
{
  const auto& test_data = GetParam();

  const auto map_directory = "test/resource/integration/tilesets"_path;
  const auto map_filename =
      fmt::format("map_with_{}_tilesets{}",
                  test_data.embed_tilesets ? "embedded" : "external",
                  test_data.file_extension);
  const auto map_path = map_directory / map_filename;

  ir::Map map {};
  map.meta.name = map_filename;

  map.tile_width = 100;
  map.tile_height = 100;

  map.row_count = 1;
  map.col_count = 1;

  map.tilesets = {
    ir::TilesetRef {
      .tileset =
          ir::Tileset {
            .meta = {},
            .tile_width = 16,
            .tile_height = 16,
            .tile_count = 240,
            .column_count = 48,
            .image_width = 768,
            .image_height = 80,
            .image_path = "image.png",
            .tiles = {},
          },
      .first_tile_id = 1,
    },
  };

  const SaveFormatWriteOptions write_options = {
    .base_dir = map_directory,
    .use_external_tilesets = !test_data.embed_tilesets,
    .use_indentation = true,
  };

  const SaveFormatReadOptions read_options = {
    .base_dir = map_directory,
    .strict_mode = false,
  };

  const auto& save_format_manager = SaveFormatManager::get();
  ASSERT_TRUE(save_format_manager.save_map(map_path, map, write_options).has_value());

  const auto parsed_map = save_format_manager.load_map(map_path, read_options);
  ASSERT_TRUE(parsed_map.has_value());

  test::expect_eq(*parsed_map, map);
}
