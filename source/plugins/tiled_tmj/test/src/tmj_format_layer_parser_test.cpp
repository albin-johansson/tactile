// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj/tmj_format_layer_parser.hpp"

#include <gtest/gtest.h>

#include "tactile/runtime/command_line_options.hpp"
#include "tactile/runtime/runtime.hpp"

#ifdef TACTILE_HAS_ZLIB_COMPRESSION
  #include "tactile/zlib/zlib_compression_plugin.hpp"
#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#ifdef TACTILE_HAS_ZSTD_COMPRESSION
  #include "tactile/zstd_compression/zstd_compression_plugin.hpp"
#endif  // TACTILE_HAS_ZSTD_COMPRESSION

namespace tactile::test {

class TmjFormatLayerParserTest : public testing::Test
{
 public:
  void SetUp() override
  {
#ifdef TACTILE_HAS_ZLIB_COMPRESSION
    mZlibPlugin.load(&mRuntime);
#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#ifdef TACTILE_HAS_ZSTD_COMPRESSION
    mZstdPlugin.load(&mRuntime);
#endif  // TACTILE_HAS_ZSTD_COMPRESSION
  }

  void TearDown() override
  {
#ifdef TACTILE_HAS_ZLIB_COMPRESSION
    mZlibPlugin.unload();
#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#ifdef TACTILE_HAS_ZSTD_COMPRESSION
    mZstdPlugin.unload();
#endif  // TACTILE_HAS_ZSTD_COMPRESSION
  }

 protected:
  Runtime mRuntime {get_default_command_line_options()};

#ifdef TACTILE_HAS_ZLIB_COMPRESSION
  ZlibCompressionPlugin mZlibPlugin {};
#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#ifdef TACTILE_HAS_ZSTD_COMPRESSION
  ZstdCompressionPlugin mZstdPlugin {};
#endif  // TACTILE_HAS_ZSTD_COMPRESSION
};

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, ParseTileLayer)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 42,
    "name": "foo",
    "opacity": 0.5,
    "visible": true,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 4,
    "height": 3,
    "data": [11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34]
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_TRUE(layer.has_value());

  EXPECT_EQ(layer->type, LayerType::kTileLayer);
  EXPECT_EQ(layer->id, ObjectID {42});
  EXPECT_EQ(layer->meta.name, "foo");
  EXPECT_EQ(layer->meta.properties.size(), 0);
  EXPECT_EQ(layer->meta.components.size(), 0);
  EXPECT_EQ(layer->opacity, 0.5f);
  EXPECT_TRUE(layer->visible);
  EXPECT_EQ(layer->extent.rows, 3);
  EXPECT_EQ(layer->extent.cols, 4);
  ASSERT_EQ(layer->tiles.size(), 3);
  ASSERT_EQ(layer->tiles[0].size(), 4);
  ASSERT_EQ(layer->tiles[1].size(), 4);
  ASSERT_EQ(layer->tiles[2].size(), 4);

  EXPECT_EQ(layer->tiles[0][0], TileID {11});
  EXPECT_EQ(layer->tiles[0][1], TileID {12});
  EXPECT_EQ(layer->tiles[0][2], TileID {13});
  EXPECT_EQ(layer->tiles[0][3], TileID {14});

  EXPECT_EQ(layer->tiles[1][0], TileID {21});
  EXPECT_EQ(layer->tiles[1][1], TileID {22});
  EXPECT_EQ(layer->tiles[1][2], TileID {23});
  EXPECT_EQ(layer->tiles[1][3], TileID {24});

  EXPECT_EQ(layer->tiles[2][0], TileID {31});
  EXPECT_EQ(layer->tiles[2][1], TileID {32});
  EXPECT_EQ(layer->tiles[2][2], TileID {33});
  EXPECT_EQ(layer->tiles[2][3], TileID {34});
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, ParseUncompressedBase64TileLayer)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 100,
    "name": "X",
    "opacity": 1,
    "visible": true,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 3,
    "height": 2,
    "encoding": "base64",
    "data": "AQAAAAIAAAADAAAABAAAAAUAAAAGAAAA"
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_TRUE(layer.has_value());

  EXPECT_EQ(layer->type, LayerType::kTileLayer);
  EXPECT_EQ(layer->id, ObjectID {100});
  EXPECT_EQ(layer->meta.name, "X");
  EXPECT_EQ(layer->meta.properties.size(), 0);
  EXPECT_EQ(layer->meta.components.size(), 0);
  EXPECT_EQ(layer->opacity, 1.0f);
  EXPECT_TRUE(layer->visible);
  EXPECT_EQ(layer->extent.rows, 2);
  EXPECT_EQ(layer->extent.cols, 3);
  ASSERT_EQ(layer->tiles.size(), 2);
  ASSERT_EQ(layer->tiles[0].size(), 3);
  ASSERT_EQ(layer->tiles[1].size(), 3);

  EXPECT_EQ(layer->tiles[0][0], TileID {1});
  EXPECT_EQ(layer->tiles[0][1], TileID {2});
  EXPECT_EQ(layer->tiles[0][2], TileID {3});
  EXPECT_EQ(layer->tiles[1][0], TileID {4});
  EXPECT_EQ(layer->tiles[1][1], TileID {5});
  EXPECT_EQ(layer->tiles[1][2], TileID {6});
}

#ifdef TACTILE_HAS_ZLIB_COMPRESSION

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, ParseZlibCompressedBase64TileLayer)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 100,
    "name": "X",
    "opacity": 1,
    "visible": true,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 3,
    "height": 2,
    "encoding": "base64",
    "compression": "zlib",
    "data": "eJxjZGBgYAJiZiBmAWJWIGYDYgAA+AAW"
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_TRUE(layer.has_value());

  EXPECT_EQ(layer->type, LayerType::kTileLayer);
  EXPECT_EQ(layer->id, ObjectID {100});
  EXPECT_EQ(layer->meta.name, "X");
  EXPECT_EQ(layer->meta.properties.size(), 0);
  EXPECT_EQ(layer->meta.components.size(), 0);
  EXPECT_EQ(layer->opacity, 1.0f);
  EXPECT_TRUE(layer->visible);
  EXPECT_EQ(layer->extent.rows, 2);
  EXPECT_EQ(layer->extent.cols, 3);
  ASSERT_EQ(layer->tiles.size(), 2);
  ASSERT_EQ(layer->tiles[0].size(), 3);
  ASSERT_EQ(layer->tiles[1].size(), 3);

  EXPECT_EQ(layer->tiles[0][0], TileID {1});
  EXPECT_EQ(layer->tiles[0][1], TileID {2});
  EXPECT_EQ(layer->tiles[0][2], TileID {3});
  EXPECT_EQ(layer->tiles[1][0], TileID {4});
  EXPECT_EQ(layer->tiles[1][1], TileID {5});
  EXPECT_EQ(layer->tiles[1][2], TileID {6});
}

#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#ifdef TACTILE_HAS_ZSTD_COMPRESSION

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, ParseZstdCompressedBase64TileLayer)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 100,
    "name": "X",
    "opacity": 1,
    "visible": true,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 3,
    "height": 2,
    "encoding": "base64",
    "compression": "zstd",
    "data": "KLUv\/SAYwQAAAQAAAAIAAAADAAAABAAAAAUAAAAGAAAA"
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_TRUE(layer.has_value());

  EXPECT_EQ(layer->type, LayerType::kTileLayer);
  EXPECT_EQ(layer->id, ObjectID {100});
  EXPECT_EQ(layer->meta.name, "X");
  EXPECT_EQ(layer->meta.properties.size(), 0);
  EXPECT_EQ(layer->meta.components.size(), 0);
  EXPECT_EQ(layer->opacity, 1.0f);
  EXPECT_TRUE(layer->visible);
  EXPECT_EQ(layer->extent.rows, 2);
  EXPECT_EQ(layer->extent.cols, 3);
  ASSERT_EQ(layer->tiles.size(), 2);
  ASSERT_EQ(layer->tiles[0].size(), 3);
  ASSERT_EQ(layer->tiles[1].size(), 3);

  EXPECT_EQ(layer->tiles[0][0], TileID {1});
  EXPECT_EQ(layer->tiles[0][1], TileID {2});
  EXPECT_EQ(layer->tiles[0][2], TileID {3});
  EXPECT_EQ(layer->tiles[1][0], TileID {4});
  EXPECT_EQ(layer->tiles[1][1], TileID {5});
  EXPECT_EQ(layer->tiles[1][2], TileID {6});
}

#endif  // TACTILE_HAS_ZSTD_COMPRESSION

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, ParseObjectLayer)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 99,
    "name": "objects",
    "opacity": 0.25,
    "visible": false,
    "type": "objectgroup",
    "x": 0,
    "y": 0,
    "draworder": "topdown",
    "objects": [
      {
        "name": "rectangle",
        "id": 1,
        "x": 10.0,
        "y": 11.0,
        "width": 12.0,
        "height": 13.0,
        "visible": true
      },
      {
        "name": "ellipse",
        "id": 2,
        "x": 10.0,
        "y": 11.0,
        "width": 12.0,
        "height": 13.0,
        "visible": true,
        "ellipse": true
      },
      {
        "name": "point",
        "id": 3,
        "x": 10.0,
        "y": 11.0,
        "width": 12.0,
        "height": 13.0,
        "visible": true,
        "point": true
      }
    ]
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_TRUE(layer.has_value());

  EXPECT_EQ(layer->type, LayerType::kObjectLayer);
  EXPECT_EQ(layer->id, ObjectID {99});
  EXPECT_EQ(layer->meta.name, "objects");
  EXPECT_EQ(layer->meta.properties.size(), 0);
  EXPECT_EQ(layer->meta.components.size(), 0);
  EXPECT_EQ(layer->opacity, 0.25f);
  EXPECT_FALSE(layer->visible);
  ASSERT_EQ(layer->objects.size(), 3);
  EXPECT_EQ(layer->objects[0].type, ObjectType::kRect);
  EXPECT_EQ(layer->objects[1].type, ObjectType::kEllipse);
  EXPECT_EQ(layer->objects[2].type, ObjectType::kPoint);
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, ParseGroupLayer)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 1,
    "name": "A",
    "opacity": 1,
    "visible": true,
    "type": "group",
    "x": 0,
    "y": 0,
    "layers": [
      {
        "id": 2,
        "name": "B",
        "opacity": 0.5,
        "visible": true,
        "type": "tilelayer",
        "x": 0,
        "y": 0,
        "width": 2,
        "height": 2,
        "data": [11, 12, 21, 22]
      },
      {
        "id": 3,
        "name": "C",
        "opacity": 0.25,
        "visible": false,
        "type": "objectgroup",
        "x": 0,
        "y": 0,
        "draworder": "topdown",
        "objects": []
      },
      {
        "id": 4,
        "name": "D",
        "opacity": 1,
        "visible": false,
        "type": "group",
        "x": 0,
        "y": 0,
        "layers": [
          {
            "id": 5,
            "name": "E",
            "opacity": 1.0,
            "visible": false,
            "type": "objectgroup",
            "x": 0,
            "y": 0,
            "draworder": "index",
            "objects": []
          }
        ]
      }
    ]
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_TRUE(layer.has_value());

  EXPECT_EQ(layer->type, LayerType::kGroupLayer);
  EXPECT_EQ(layer->id, ObjectID {1});
  EXPECT_EQ(layer->meta.name, "A");
  EXPECT_EQ(layer->meta.properties.size(), 0);
  EXPECT_EQ(layer->meta.components.size(), 0);
  EXPECT_EQ(layer->opacity, 1.0f);
  EXPECT_TRUE(layer->visible);
  ASSERT_EQ(layer->layers.size(), 3);

  const auto& layer2 = layer->layers[0];
  const auto& layer3 = layer->layers[1];
  const auto& layer4 = layer->layers[2];

  EXPECT_EQ(layer2.id, LayerID {2});
  EXPECT_EQ(layer2.type, LayerType::kTileLayer);

  EXPECT_EQ(layer3.id, LayerID {3});
  EXPECT_EQ(layer3.type, LayerType::kObjectLayer);

  EXPECT_EQ(layer4.id, LayerID {4});
  EXPECT_EQ(layer4.type, LayerType::kGroupLayer);

  ASSERT_EQ(layer4.layers.size(), 1);
  const auto& layer5 = layer4.layers[0];

  EXPECT_EQ(layer5.id, LayerID {5});
  EXPECT_EQ(layer5.type, LayerType::kObjectLayer);
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, LayerWithoutId)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "name": "",
    "opacity": 0.0,
    "visible": false,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 0,
    "height": 0,
    "data": []
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_FALSE(layer.has_value());

  EXPECT_EQ(layer.error(), SaveFormatParseError::kNoLayerId);
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, LayerWithoutName)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 0,
    "opacity": 0.0,
    "visible": false,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 0,
    "height": 0,
    "data": []
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_FALSE(layer.has_value());

  EXPECT_EQ(layer.error(), SaveFormatParseError::kNoLayerName);
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, LayerWithoutOpacity)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 0,
    "name": "",
    "visible": false,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 0,
    "height": 0,
    "data": []
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_FALSE(layer.has_value());

  EXPECT_EQ(layer.error(), SaveFormatParseError::kNoLayerOpacity);
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, LayerWithoutVisibility)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 0,
    "name": "",
    "opacity": 0.0,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 0,
    "height": 0,
    "data": []
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_FALSE(layer.has_value());

  EXPECT_EQ(layer.error(), SaveFormatParseError::kNoLayerVisibility);
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, LayerWithoutType)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 0,
    "name": "",
    "opacity": 0.0,
    "visible": false,
    "x": 0,
    "y": 0,
    "width": 0,
    "height": 0,
    "data": []
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_FALSE(layer.has_value());

  EXPECT_EQ(layer.error(), SaveFormatParseError::kNoLayerType);
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, TileLayerWithoutWidth)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 0,
    "name": "",
    "opacity": 0.0,
    "visible": false,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "height": 0,
    "data": []
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_FALSE(layer.has_value());

  EXPECT_EQ(layer.error(), SaveFormatParseError::kNoTileLayerWidth);
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, TileLayerWithoutHeight)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 0,
    "name": "",
    "opacity": 0.0,
    "visible": false,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 0,
    "data": []
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_FALSE(layer.has_value());

  EXPECT_EQ(layer.error(), SaveFormatParseError::kNoTileLayerHeight);
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, TileLayerWithBadEncoding)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 0,
    "name": "",
    "opacity": 0.0,
    "visible": false,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 0,
    "height": 0,
    "encoding": "foo",
    "data": []
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_FALSE(layer.has_value());

  EXPECT_EQ(layer.error(), SaveFormatParseError::kBadTileLayerEncoding);
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, TileLayerWithTooFewTiles)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 42,
    "name": "foo",
    "opacity": 0.5,
    "visible": true,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 4,
    "height": 3,
    "data": [11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33]
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_FALSE(layer.has_value());

  EXPECT_EQ(layer.error(), SaveFormatParseError::kBadTileLayerData);
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, TileLayerWithTooManyTiles)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 42,
    "name": "foo",
    "opacity": 0.5,
    "visible": true,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 4,
    "height": 3,
    "data": [11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 35]
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_FALSE(layer.has_value());

  EXPECT_EQ(layer.error(), SaveFormatParseError::kBadTileLayerData);
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, TileLayerWithInvalidCompressionFormat)
{
  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 1,
    "name": "",
    "opacity": 1,
    "visible": true,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 1,
    "height": 1,
    "encoding": "base64",
    "compression": "foo",
    "data": ""
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_FALSE(layer.has_value());

  EXPECT_EQ(layer.error(), SaveFormatParseError::kNoSuchCompressionFormat);
}

// tactile::parse_tiled_tmj_layer
TEST_F(TmjFormatLayerParserTest, TileLayerWithUnregisteredCompressionFormat)
{
  mRuntime.set_compression_format(CompressionFormat::kZlib, nullptr);

  using namespace nlohmann::json_literals;

  const auto layer_json = R"({
    "id": 1,
    "name": "",
    "opacity": 1,
    "visible": true,
    "type": "tilelayer",
    "x": 0,
    "y": 0,
    "width": 1,
    "height": 1,
    "encoding": "base64",
    "compression": "zlib",
    "data": ""
  })"_json;

  const auto layer = parse_tiled_tmj_layer(mRuntime, layer_json);
  ASSERT_FALSE(layer.has_value());

  EXPECT_EQ(layer.error(), SaveFormatParseError::kNoSuchCompressionFormat);
}

}  // namespace tactile::test
