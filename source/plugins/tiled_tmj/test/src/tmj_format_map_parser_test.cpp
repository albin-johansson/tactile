// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj/tmj_format_map_parser.hpp"

#include <gtest/gtest.h>

#include "tactile/runtime/command_line_options.hpp"
#include "tactile/runtime/runtime.hpp"

namespace tactile::test {

class TmjFormatMapParserTest : public testing::Test
{
 protected:
  Runtime mRuntime {get_default_command_line_options()};
  SaveFormatReadOptions mOptions {};
};

// tactile::parse_tiled_tmj_map
TEST_F(TmjFormatMapParserTest, ParseEmptyMap)
{
  using namespace nlohmann::json_literals;

  const auto map_json = R"({
    "orientation": "orthogonal",
    "name": "",
    "width": 5,
    "height": 4,
    "tilewidth": 50,
    "tileheight": 51,
    "nextlayerid": 10,
    "nextobjectid": 20
  })"_json;

  const auto map = parse_tiled_tmj_map(mRuntime, map_json, mOptions);
  ASSERT_TRUE(map.has_value());

  EXPECT_EQ(map->extent.rows, 4);
  EXPECT_EQ(map->extent.cols, 5);
  EXPECT_EQ(map->tile_size.x(), 50);
  EXPECT_EQ(map->tile_size.y(), 51);
  EXPECT_EQ(map->next_layer_id, 10);
  EXPECT_EQ(map->next_object_id, 20);
  EXPECT_EQ(map->tile_format.encoding, TileEncoding::kPlainText);
  EXPECT_EQ(map->tile_format.compression, std::nullopt);
  EXPECT_EQ(map->tile_format.compression_level, std::nullopt);
  EXPECT_EQ(map->components.size(), 0);
  EXPECT_EQ(map->tilesets.size(), 0);
  EXPECT_EQ(map->layers.size(), 0);
}

// tactile::parse_tiled_tmj_map
TEST_F(TmjFormatMapParserTest, ParseOrthogonalMap)
{
  using namespace nlohmann::json_literals;

  const auto map_json = R"({
    "orientation": "orthogonal",
    "name": "",
    "width": 2,
    "height": 2,
    "tilewidth": 50,
    "tileheight": 50,
    "nextlayerid": 3,
    "nextobjectid": 1,
    "layers": [
      {
        "id": 1,
        "name": "Ground",
        "type": "tilelayer",
        "opacity": 1,
        "visible": true,
        "x": 0,
        "y": 0,
        "width": 2,
        "height": 2,
        "data": [1, 2, 3, 4]
      },
      {
        "id": 2,
        "name": "Details",
        "type": "tilelayer",
        "opacity": 1,
        "visible": true,
        "x": 0,
        "y": 0,
        "width": 2,
        "height": 2,
        "data": [4, 0, 2, 0]
      }
    ],
    "tilesets": [
      {
        "firstgid": 1,
        "name": "dummy",
        "columns": 6,
        "tilewidth": 16,
        "tileheight": 16,
        "tilecount": 24,
        "imagewidth": 96,
        "imageheight": 64,
        "image": "assets/images/dummy.png",
        "spacing": 0,
        "margin": 0
      }
    ]
  })"_json;

  const auto map = parse_tiled_tmj_map(mRuntime, map_json, mOptions);
  ASSERT_TRUE(map.has_value());

  EXPECT_EQ(map->extent.rows, 2);
  EXPECT_EQ(map->extent.cols, 2);
  EXPECT_EQ(map->tile_size.x(), 50);
  EXPECT_EQ(map->tile_size.y(), 50);
  EXPECT_EQ(map->next_layer_id, 3);
  EXPECT_EQ(map->next_object_id, 1);
  EXPECT_EQ(map->tile_format.encoding, TileEncoding::kPlainText);
  EXPECT_EQ(map->tile_format.compression, std::nullopt);
  EXPECT_EQ(map->tile_format.compression_level, std::nullopt);
  ASSERT_EQ(map->components.size(), 0);
  ASSERT_EQ(map->tilesets.size(), 1);
  ASSERT_EQ(map->layers.size(), 2);
}

// tactile::parse_tiled_tmj_map
TEST_F(TmjFormatMapParserTest, MapWithoutOrientation)
{
  using namespace nlohmann::json_literals;

  const auto map_json = R"({
    "name": "",
    "width": 2,
    "height": 2,
    "tilewidth": 50,
    "tileheight": 50,
    "nextlayerid": 3,
    "nextobjectid": 1
  })"_json;

  const auto map = parse_tiled_tmj_map(mRuntime, map_json, mOptions);
  ASSERT_FALSE(map.has_value());

  EXPECT_EQ(map.error(), SaveFormatParseError::kNoMapOrientation);
}

// tactile::parse_tiled_tmj_map
TEST_F(TmjFormatMapParserTest, MapWithUnsupportedOrientation)
{
  using namespace nlohmann::json_literals;

  const auto map_json = R"({
    "orientation": "isometric",
    "name": "",
    "width": 2,
    "height": 2,
    "tilewidth": 50,
    "tileheight": 50,
    "nextlayerid": 3,
    "nextobjectid": 1
  })"_json;

  const auto map = parse_tiled_tmj_map(mRuntime, map_json, mOptions);
  ASSERT_FALSE(map.has_value());

  EXPECT_EQ(map.error(), SaveFormatParseError::kBadMapOrientation);
}

// tactile::parse_tiled_tmj_map
TEST_F(TmjFormatMapParserTest, MapWithoutWidth)
{
  using namespace nlohmann::json_literals;

  const auto map_json = R"({
    "orientation": "orthogonal",
    "name": "",
    "height": 2,
    "tilewidth": 50,
    "tileheight": 50,
    "nextlayerid": 3,
    "nextobjectid": 1
  })"_json;

  const auto map = parse_tiled_tmj_map(mRuntime, map_json, mOptions);
  ASSERT_FALSE(map.has_value());

  EXPECT_EQ(map.error(), SaveFormatParseError::kNoMapWidth);
}

// tactile::parse_tiled_tmj_map
TEST_F(TmjFormatMapParserTest, MapWithoutHeight)
{
  using namespace nlohmann::json_literals;

  const auto map_json = R"({
    "orientation": "orthogonal",
    "name": "",
    "width": 2,
    "tilewidth": 50,
    "tileheight": 50,
    "nextlayerid": 3,
    "nextobjectid": 1
  })"_json;

  const auto map = parse_tiled_tmj_map(mRuntime, map_json, mOptions);
  ASSERT_FALSE(map.has_value());

  EXPECT_EQ(map.error(), SaveFormatParseError::kNoMapHeight);
}

// tactile::parse_tiled_tmj_map
TEST_F(TmjFormatMapParserTest, MapWithoutTileWidth)
{
  using namespace nlohmann::json_literals;

  const auto map_json = R"({
    "orientation": "orthogonal",
    "name": "",
    "width": 2,
    "height": 2,
    "tileheight": 50,
    "nextlayerid": 3,
    "nextobjectid": 1
  })"_json;

  const auto map = parse_tiled_tmj_map(mRuntime, map_json, mOptions);
  ASSERT_FALSE(map.has_value());

  EXPECT_EQ(map.error(), SaveFormatParseError::kNoMapTileWidth);
}

// tactile::parse_tiled_tmj_map
TEST_F(TmjFormatMapParserTest, MapWithoutTileHeight)
{
  using namespace nlohmann::json_literals;

  const auto map_json = R"({
    "orientation": "orthogonal",
    "name": "",
    "width": 2,
    "height": 2,
    "tilewidth": 50,
    "nextlayerid": 3,
    "nextobjectid": 1
  })"_json;

  const auto map = parse_tiled_tmj_map(mRuntime, map_json, mOptions);
  ASSERT_FALSE(map.has_value());

  EXPECT_EQ(map.error(), SaveFormatParseError::kNoMapTileHeight);
}

// tactile::parse_tiled_tmj_map
TEST_F(TmjFormatMapParserTest, MapWithoutNextLayerId)
{
  using namespace nlohmann::json_literals;

  const auto map_json = R"({
    "orientation": "orthogonal",
    "name": "",
    "width": 2,
    "height": 2,
    "tilewidth": 50,
    "tileheight": 50,
    "nextobjectid": 1
  })"_json;

  const auto map = parse_tiled_tmj_map(mRuntime, map_json, mOptions);
  ASSERT_FALSE(map.has_value());

  EXPECT_EQ(map.error(), SaveFormatParseError::kNoMapNextLayerId);
}

// tactile::parse_tiled_tmj_map
TEST_F(TmjFormatMapParserTest, MapWithoutNextObjectId)
{
  using namespace nlohmann::json_literals;

  const auto map_json = R"({
    "orientation": "orthogonal",
    "name": "",
    "width": 2,
    "height": 2,
    "tilewidth": 50,
    "tileheight": 50,
    "nextlayerid": 1
  })"_json;

  const auto map = parse_tiled_tmj_map(mRuntime, map_json, mOptions);
  ASSERT_FALSE(map.has_value());

  EXPECT_EQ(map.error(), SaveFormatParseError::kNoMapNextObjectId);
}

}  // namespace tactile::test
