// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-json-format/emit/layer_emitter.hpp"

#include <gtest/gtest.h>

#include "tactile/foundation/misc/conversion.hpp"

using namespace tactile;

TEST(TmjLayerEmitter, EmitTileLayer)
{
  const ir::Layer layer {
    .meta =
        ir::Metadata {
          .properties = {},
          .components = {},
        },
    .name = "Tile layer",
    .id = 123,
    .type = LayerType::kTileLayer,
    .opacity = 0.5f,
    .row_count = 4,
    .col_count = 2,
    .tiles =
        TileMatrix {
          {TileID {1}, TileID {2}},
          {TileID {3}, TileID {4}},
          {TileID {5}, TileID {6}},
          {TileID {7}, TileID {8}},
        },
    .objects = {},
    .layers = {},
    .visible = true,
  };

  const ir::TileFormat tile_format {
    .encoding = TileEncoding::kPlainText,
    .compression = CompressionMode::kNone,
    .compression_level = kNone,
  };

  const auto layer_json = tiled::tmj::emit_layer(layer, tile_format);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("name"), layer.name);
  EXPECT_EQ(layer_json->at("id"), layer.id);
  EXPECT_EQ(layer_json->at("opacity"), layer.opacity);
  EXPECT_EQ(layer_json->at("visible"), layer.visible);

  EXPECT_FALSE(layer_json->contains("objects"));
  EXPECT_FALSE(layer_json->contains("layers"));

  EXPECT_EQ(layer_json->at("type"), "tilelayer");
  EXPECT_EQ(layer_json->at("width"), layer.col_count);
  EXPECT_EQ(layer_json->at("height"), layer.row_count);

  const auto& tile_data_json_array = layer_json->at("data");
  ASSERT_TRUE(tile_data_json_array.is_array());
  ASSERT_EQ(tile_data_json_array.size(), layer.row_count * layer.col_count);

  const auto row_count = as_unsigned(layer.row_count);
  const auto col_count = as_unsigned(layer.col_count);
  for (usize row = 0; row < row_count; ++row) {
    for (usize col = 0; col < col_count; ++col) {
      const usize tile_index = (row * col_count) + col;
      EXPECT_EQ(tile_data_json_array.at(tile_index), layer.tiles[row][col].value);
    }
  }
}

TEST(TmjLayerEmitter, EmitObjectLayer)
{
  const ir::Layer layer {
    .meta =
        ir::Metadata {
          .properties = {},
          .components = {},
        },
    .name = "Object layer",
    .id = 77,
    .type = LayerType::kObjectLayer,
    .opacity = 1.0f,
    .row_count = 0,
    .col_count = 0,
    .tiles = {},
    .objects =
        {
          ir::Object {},
          ir::Object {},
          ir::Object {},
        },
    .layers = {},
    .visible = false,
  };

  const ir::TileFormat tile_format {
    .encoding = TileEncoding::kPlainText,
    .compression = CompressionMode::kNone,
    .compression_level = kNone,
  };

  const auto layer_json = tiled::tmj::emit_layer(layer, tile_format);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("name"), layer.name);
  EXPECT_EQ(layer_json->at("id"), layer.id);
  EXPECT_EQ(layer_json->at("opacity"), layer.opacity);
  EXPECT_EQ(layer_json->at("visible"), layer.visible);

  EXPECT_FALSE(layer_json->contains("width"));
  EXPECT_FALSE(layer_json->contains("height"));
  EXPECT_FALSE(layer_json->contains("data"));
  EXPECT_FALSE(layer_json->contains("layers"));

  EXPECT_EQ(layer_json->at("type"), "objectgroup");

  const auto& object_data_json_array = layer_json->at("objects");
  ASSERT_TRUE(object_data_json_array.is_array());
  ASSERT_EQ(object_data_json_array.size(), layer.objects.size());

  // TODO compare objects
}

// TODO EmitGroupLayer
