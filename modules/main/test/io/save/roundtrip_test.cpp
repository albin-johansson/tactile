// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <utility>  // move

#include <fmt/format.h>
#include <gtest/gtest.h>

#include "tactile/core/container/string.hpp"
#include "tactile/core/io/save/save_format_manager.hpp"
#include "test/ir_comparison.hpp"

using namespace tactile;
using namespace tactile::fs_literals;

class SaveFormatManagerTest : public testing::TestWithParam<String> {};

INSTANTIATE_TEST_SUITE_P(Roundtrip, SaveFormatManagerTest, testing::Values(".tmj"));

namespace {

inline const auto kMapDirectory = "test/resource/integration/roundtrip"_path;

inline constexpr usize kRowCount = 4;
inline constexpr usize kColCount = 5;

inline constexpr ir::TileFormat kTileFormat {
  .encoding = TileEncoding::kPlainText,
  .compression = CompressionMode::kNone,
  .zlib_level = kNone,
  .zstd_level = kNone,
};

inline const ir::Object kPointObject {
  .meta =
      {
        .name = "kPointObject",
        .properties =
            {
              ir::NamedAttribute {.name = "kPointObject::int", .value = -48'921},
            },
        .components = {},
      },
  .id = ObjectID {100},
  .type = ObjectType::kPoint,
  .x = 8'231,
  .y = 5'000,
  .width = 0,
  .height = 0,
  .tag = "tag::point",
  .visible = false,
};

inline const ir::Object kRectObject {
  .meta =
      {
        .name = "kRectObject",
        .properties = {},
        .components = {},
      },
  .id = ObjectID {84},
  .type = ObjectType::kRect,
  .x = 532,
  .y = 834,
  .width = 100,
  .height = 80,
  .tag = "tag::rect",
  .visible = true,
};

inline const ir::Object kEllipseObject {
  .meta =
      {
        .name = "kEllipseObject",
        .properties = {},
        .components = {},
      },
  .id = ObjectID {999},
  .type = ObjectType::kEllipse,
  .x = -1'283,
  .y = 634,
  .width = 183,
  .height = 155,
  .tag = "tag::ellipse",
  .visible = true,
};

inline const ir::Tileset kTileset1 {
  .meta =
      {
        .name = "TS1",
        .properties =
            {
              {.name = "TS1::bool", .value = false},
            },
        .components = {},
      },
  .tile_width = 16,
  .tile_height = 24,
  .tile_count = 128,
  .column_count = 64,
  .image_width = 1024,
  .image_height = 768,
  .image_path = "foo.png",
  .tiles = {},
};

inline const ir::Tile kTileset2Tile1 {
  .meta =
      {
        .name = "",
        .properties = {},
        .components = {},
      },
  .index = TileIndex {0},
  .objects = {},
  .animation =
      {
        ir::AnimationFrame {.tile_index = TileIndex {100}, .duration_ms = 743},
        ir::AnimationFrame {.tile_index = TileIndex {101}, .duration_ms = 859},
        ir::AnimationFrame {.tile_index = TileIndex {941}, .duration_ms = 457},
      },
};

inline const ir::Tile kTileset2Tile2 {
  .meta =
      {
        .name = "",
        .properties = {},
        .components = {},
      },
  .index = TileIndex {1},
  .objects = {kRectObject, kEllipseObject},
  .animation = {},
};

inline const ir::Tileset kTileset2 {
  .meta =
      {
        .name = "TS2",
        .properties = {},
        .components = {},
      },
  .tile_width = 28,
  .tile_height = 32,
  .tile_count = 64,
  .column_count = 19,
  .image_width = 500,
  .image_height = 1000,
  .image_path = "../images/bar.png",
  .tiles = {kTileset2Tile1, kTileset2Tile2},
};

inline const ir::Layer kLayer1 {
  .meta =
      {
        .name = "T1",
        .properties = {},
        .components = {},
      },
  .id = 10,
  .type = LayerType::kTileLayer,
  .opacity = 1.0f,
  .width = kColCount,
  .height = kRowCount,
  .tiles = make_tile_matrix(kRowCount, kColCount),
  .objects = {},
  .layers = {},
  .visible = true,
};

inline const ir::Layer kObjectLayer {
  .meta =
      {
        .name = "O1",
        .properties =
            {
              {.name = "O1::int", .value = 999},
            },
        .components = {},
      },
  .id = 30,
  .type = LayerType::kObjectLayer,
  .opacity = 0.8f,
  .width = 0,
  .height = 0,
  .tiles = {},
  .objects = {kPointObject, kRectObject, kEllipseObject},
  .layers = {},
  .visible = true,
};

inline const ir::Layer kGroupLayer {
  .meta =
      {
        .name = "G1",
        .properties =
            {
              {.name = "G1::float", .value = 15.9f},
              {.name = "G1::string", .value = "foobar"},
            },
        .components = {},
      },
  .id = 20,
  .type = LayerType::kGroupLayer,
  .opacity = 1.0f,
  .width = 0,
  .height = 0,
  .tiles = {},
  .objects = {},
  .layers = {kObjectLayer},
  .visible = true,
};

}  // namespace

// Tries to save and restore a map that uses a common subset of the save format features.
TEST_P(SaveFormatManagerTest, SaveAndLoadMap)
{
  const String& file_extension = GetParam();

  const auto map_filename = fmt::format("map{}", file_extension);
  const auto map_path = kMapDirectory / map_filename;

  const ir::Map map {
    .meta =
        {
          .name = map_filename,
          .properties =
              {
                {.name = "Map::str", .value = "foo"},
                {.name = "Map::int", .value = 42},
                {.name = "Map::float", .value = 1.0f},
                {.name = "Map::bool", .value = true},
                {.name = "Map::path", .value = "foo/bar.test"_path},
                {.name = "Map::object", .value = ObjectRef {7}},
                {.name = "Map::color", .value = UColor {0xDE, 0xAD, 0xBE, 0xEF}},
              },
          .components = {},
        },
    .row_count = kRowCount,
    .col_count = kColCount,
    .tile_width = 30,
    .tile_height = 32,
    .next_layer_id = 10,
    .next_object_id = 20,
    .tile_format = kTileFormat,
    .components = {},
    .tilesets =
        {
          ir::TilesetRef {.tileset = kTileset1, .first_tile_id = TileID {1}},
          ir::TilesetRef {.tileset = kTileset2, .first_tile_id = TileID {129}},
        },
    .layers = {kLayer1, kGroupLayer},
  };

  const SaveFormatWriteOptions write_options = {
    .base_dir = map_path.parent_path(),
    .use_external_tilesets = false,
    .use_indentation = true,
  };

  const SaveFormatReadOptions read_options = {
    .base_dir = map_path.parent_path(),
    .strict_mode = false,
  };

  const auto& save_format_manager = SaveFormatManager::get();
  ASSERT_TRUE(save_format_manager.save_map(map_path, map, write_options).has_value());

  const auto parsed_map = save_format_manager.load_map(map_path, read_options);
  ASSERT_TRUE(parsed_map.has_value());

  test::expect_eq(*parsed_map, map);
}
