// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj/tmj_format_tileset_parser.hpp"

#include <gtest/gtest.h>

#include "tactile/runtime/command_line_options.hpp"
#include "tactile/runtime/runtime.hpp"

namespace tactile::test {

class TmjFormatTilesetParserTest : public testing::Test
{
 protected:
  Runtime mRuntime {get_default_command_line_options()};
  SaveFormatReadOptions mOptions {};
};

// tactile::parse_tiled_tmj_tileset
TEST_F(TmjFormatTilesetParserTest, ValidEmbeddedTileset)
{
  using namespace nlohmann::json_literals;

  const auto tileset_json = R"({
    "name": "demo",
    "firstgid": 7,
    "columns": 6,
    "tilewidth": 16,
    "tileheight": 16,
    "tilecount": 24,
    "imagewidth": 96,
    "imageheight": 64,
    "image": "assets/images/dummy.png",
    "spacing": 0,
    "margin": 0,
    "tiles": [
      {
        "id": 5
      },
      {
        "id": 6,
        "objectgroup": {
          "draworder": "topdown",
          "type": "objectgroup",
          "name": "",
          "opacity": 1,
          "visible": true,
          "x": 0,
          "y": 0,
          "objects": [
            {
              "name": "point",
              "id": 10,
              "x": 1.0,
              "y": 2.0,
              "width": 3.0,
              "height": 4.0,
              "visible": true,
              "point": true
            }
          ]
        }
      },
      {
        "id": 7,
        "animation": [
          {
            "tileid": 7,
            "duration": 200
          },
          {
            "tileid": 8,
            "duration": 300
          },
          {
            "tileid": 9,
            "duration": 400
          }
        ]
      }
    ]
  })"_json;

  const auto tileset = parse_tiled_tmj_tileset(tileset_json, mOptions);
  ASSERT_TRUE(tileset.has_value());

  EXPECT_EQ(tileset->first_tile_id, TileID {7});
  EXPECT_EQ(tileset->tileset.meta.name, "demo");
  EXPECT_EQ(tileset->tileset.meta.properties.size(), 0);
  EXPECT_EQ(tileset->tileset.meta.components.size(), 0);
  EXPECT_EQ(tileset->tileset.column_count, 6);
  EXPECT_EQ(tileset->tileset.tile_size.x(), 16);
  EXPECT_EQ(tileset->tileset.tile_size.y(), 16);
  EXPECT_EQ(tileset->tileset.tile_count, 24);
  EXPECT_EQ(tileset->tileset.image_size.x(), 96);
  EXPECT_EQ(tileset->tileset.image_size.y(), 64);
  EXPECT_EQ(tileset->tileset.image_path, "assets/images/dummy.png");
  EXPECT_TRUE(tileset->tileset.is_embedded);

  ASSERT_EQ(tileset->tileset.tiles.size(), 3);

  const auto& tile5 = tileset->tileset.tiles[0];
  const auto& tile6 = tileset->tileset.tiles[1];
  const auto& tile7 = tileset->tileset.tiles[2];

  EXPECT_EQ(tile5.index, TileIndex {5});
  EXPECT_EQ(tile5.meta.name, "");
  EXPECT_EQ(tile5.meta.properties.size(), 0);
  EXPECT_EQ(tile5.meta.components.size(), 0);
  EXPECT_EQ(tile5.animation.size(), 0);
  EXPECT_EQ(tile5.objects.size(), 0);

  EXPECT_EQ(tile6.index, TileIndex {6});
  EXPECT_EQ(tile6.meta.name, "");
  EXPECT_EQ(tile6.meta.properties.size(), 0);
  EXPECT_EQ(tile6.meta.components.size(), 0);
  EXPECT_EQ(tile6.animation.size(), 0);
  ASSERT_EQ(tile6.objects.size(), 1);
  EXPECT_EQ(tile6.objects[0].id, ObjectID {10});
  EXPECT_EQ(tile6.objects[0].type, ObjectType::kPoint);

  EXPECT_EQ(tile7.index, TileIndex {7});
  EXPECT_EQ(tile7.meta.name, "");
  EXPECT_EQ(tile7.meta.properties.size(), 0);
  EXPECT_EQ(tile7.meta.components.size(), 0);
  ASSERT_EQ(tile7.animation.size(), 3);
  EXPECT_EQ(tile7.objects.size(), 0);
  EXPECT_EQ(tile7.animation[0].tile_index, TileIndex {7});
  EXPECT_EQ(tile7.animation[0].duration, Milliseconds {200});
  EXPECT_EQ(tile7.animation[1].tile_index, TileIndex {8});
  EXPECT_EQ(tile7.animation[1].duration, Milliseconds {300});
  EXPECT_EQ(tile7.animation[2].tile_index, TileIndex {9});
  EXPECT_EQ(tile7.animation[2].duration, Milliseconds {400});
}

// tactile::parse_tiled_tmj_tileset
TEST_F(TmjFormatTilesetParserTest, TilesetWithoutFirstTileId)
{
  using namespace nlohmann::json_literals;

  const auto tileset_json = R"({
    "name": "demo",
    "columns": 6,
    "tilewidth": 16,
    "tileheight": 16,
    "tilecount": 24,
    "imagewidth": 96,
    "imageheight": 64,
    "image": "assets/images/dummy.png",
    "spacing": 0,
    "margin": 0
  })"_json;

  const auto tileset = parse_tiled_tmj_tileset(tileset_json, mOptions);
  ASSERT_FALSE(tileset.has_value());

  EXPECT_EQ(tileset.error(), SaveFormatParseError::kNoTilesetFirstTileId);
}

// tactile::parse_tiled_tmj_tileset
TEST_F(TmjFormatTilesetParserTest, TilesetWithoutName)
{
  using namespace nlohmann::json_literals;

  const auto tileset_json = R"({
    "firstgid": 1,
    "columns": 6,
    "tilewidth": 16,
    "tileheight": 16,
    "tilecount": 24,
    "imagewidth": 96,
    "imageheight": 64,
    "image": "assets/images/dummy.png",
    "spacing": 0,
    "margin": 0
  })"_json;

  const auto tileset = parse_tiled_tmj_tileset(tileset_json, mOptions);
  ASSERT_FALSE(tileset.has_value());

  EXPECT_EQ(tileset.error(), SaveFormatParseError::kNoTilesetName);
}

// tactile::parse_tiled_tmj_tileset
TEST_F(TmjFormatTilesetParserTest, TilesetWithoutTileWidth)
{
  using namespace nlohmann::json_literals;

  const auto tileset_json = R"({
    "name": "demo",
    "firstgid": 1,
    "columns": 6,
    "tileheight": 16,
    "tilecount": 24,
    "imagewidth": 96,
    "imageheight": 64,
    "image": "assets/images/dummy.png",
    "spacing": 0,
    "margin": 0
  })"_json;

  const auto tileset = parse_tiled_tmj_tileset(tileset_json, mOptions);
  ASSERT_FALSE(tileset.has_value());

  EXPECT_EQ(tileset.error(), SaveFormatParseError::kNoTilesetTileWidth);
}

// tactile::parse_tiled_tmj_tileset
TEST_F(TmjFormatTilesetParserTest, TilesetWithoutTileHeight)
{
  using namespace nlohmann::json_literals;

  const auto tileset_json = R"({
    "name": "demo",
    "firstgid": 1,
    "columns": 6,
    "tilewidth": 16,
    "tilecount": 24,
    "imagewidth": 96,
    "imageheight": 64,
    "image": "assets/images/dummy.png",
    "spacing": 0,
    "margin": 0
  })"_json;

  const auto tileset = parse_tiled_tmj_tileset(tileset_json, mOptions);
  ASSERT_FALSE(tileset.has_value());

  EXPECT_EQ(tileset.error(), SaveFormatParseError::kNoTilesetTileHeight);
}

// tactile::parse_tiled_tmj_tileset
TEST_F(TmjFormatTilesetParserTest, TilesetWithoutTileCount)
{
  using namespace nlohmann::json_literals;

  const auto tileset_json = R"({
    "name": "demo",
    "firstgid": 1,
    "columns": 6,
    "tilewidth": 16,
    "tileheight": 16,
    "imagewidth": 96,
    "imageheight": 64,
    "image": "assets/images/dummy.png",
    "spacing": 0,
    "margin": 0
  })"_json;

  const auto tileset = parse_tiled_tmj_tileset(tileset_json, mOptions);
  ASSERT_FALSE(tileset.has_value());

  EXPECT_EQ(tileset.error(), SaveFormatParseError::kNoTilesetTileCount);
}

// tactile::parse_tiled_tmj_tileset
TEST_F(TmjFormatTilesetParserTest, TilesetWithoutColumns)
{
  using namespace nlohmann::json_literals;

  const auto tileset_json = R"({
    "name": "demo",
    "firstgid": 1,
    "tilewidth": 16,
    "tileheight": 16,
    "tilecount": 24,
    "imagewidth": 96,
    "imageheight": 64,
    "image": "assets/images/dummy.png",
    "spacing": 0,
    "margin": 0
  })"_json;

  const auto tileset = parse_tiled_tmj_tileset(tileset_json, mOptions);
  ASSERT_FALSE(tileset.has_value());

  EXPECT_EQ(tileset.error(), SaveFormatParseError::kNoTilesetColumns);
}

// tactile::parse_tiled_tmj_tileset
TEST_F(TmjFormatTilesetParserTest, TilesetWithoutImageWidth)
{
  using namespace nlohmann::json_literals;

  const auto tileset_json = R"({
    "name": "demo",
    "firstgid": 1,
    "columns": 6,
    "tilewidth": 16,
    "tileheight": 16,
    "tilecount": 24,
    "imageheight": 64,
    "image": "assets/images/dummy.png",
    "spacing": 0,
    "margin": 0
  })"_json;

  const auto tileset = parse_tiled_tmj_tileset(tileset_json, mOptions);
  ASSERT_FALSE(tileset.has_value());

  EXPECT_EQ(tileset.error(), SaveFormatParseError::kNoTilesetImageWidth);
}

// tactile::parse_tiled_tmj_tileset
TEST_F(TmjFormatTilesetParserTest, TilesetWithoutImageHeight)
{
  using namespace nlohmann::json_literals;

  const auto tileset_json = R"({
    "name": "demo",
    "firstgid": 1,
    "columns": 6,
    "tilewidth": 16,
    "tileheight": 16,
    "tilecount": 24,
    "imagewidth": 96,
    "image": "assets/images/dummy.png",
    "spacing": 0,
    "margin": 0
  })"_json;

  const auto tileset = parse_tiled_tmj_tileset(tileset_json, mOptions);
  ASSERT_FALSE(tileset.has_value());

  EXPECT_EQ(tileset.error(), SaveFormatParseError::kNoTilesetImageHeight);
}

// tactile::parse_tiled_tmj_tileset
TEST_F(TmjFormatTilesetParserTest, TilesetWithoutImage)
{
  using namespace nlohmann::json_literals;

  const auto tileset_json = R"({
    "name": "demo",
    "firstgid": 1,
    "columns": 6,
    "tilewidth": 16,
    "tileheight": 16,
    "tilecount": 24,
    "imagewidth": 96,
    "imageheight": 96,
    "spacing": 0,
    "margin": 0
  })"_json;

  const auto tileset = parse_tiled_tmj_tileset(tileset_json, mOptions);
  ASSERT_FALSE(tileset.has_value());

  EXPECT_EQ(tileset.error(), SaveFormatParseError::kNoTilesetImage);
}

}  // namespace tactile::test
