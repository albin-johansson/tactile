// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj/tmj_format_layer_emitter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/base/io/tile_io.hpp"
#include "tactile/base/platform/bits.hpp"
#include "tactile/runtime/command_line_options.hpp"
#include "tactile/runtime/runtime.hpp"
#include "tactile/test_util/document_view_mocks.hpp"
#include "tactile/test_util/ir.hpp"

#ifdef TACTILE_HAS_ZLIB_COMPRESSION
  #include "tactile/zlib_compression/zlib_compression_plugin.hpp"
#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#ifdef TACTILE_HAS_ZSTD_COMPRESSION
  #include "tactile/zstd_compression/zstd_compression_plugin.hpp"
#endif  // TACTILE_HAS_ZSTD_COMPRESSION

namespace tactile::test {

class TmjFormatLayerEmitterTest : public testing::Test
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

  ByteStream mTileByteStream {};
};

// tactile::emit_tiled_tmj_layer
TEST_F(TmjFormatLayerEmitterTest, EmitPlainTileLayer)
{
  const ir::Layer ir_layer {
    .meta =
        ir::Metadata {
          .name = "plain",
          .properties = {},
          .components = {},
        },
    .id = LayerID {34},
    .type = LayerType::kTileLayer,
    .opacity = 0.1f,
    .extent = Extent2D {3, 3},
    .tiles =
        TileMatrix {
          {TileID {123}, TileID {123}, TileID {123}},
          {TileID {123}, TileID {123}, TileID {123}},
          {TileID {123}, TileID {123}, TileID {123}},
        },
    .objects = {},
    .layers = {},
    .visible = false,
  };

  const ir::TileFormat ir_tile_format {
    .encoding = TileEncoding::kPlainText,
    .compression = std::nullopt,
    .compression_level = std::nullopt,
  };

  testing::NiceMock<LayerViewMock> layer {ir_layer, ir_tile_format};
  EXPECT_CALL(layer, write_tile_bytes).Times(0);
  EXPECT_CALL(layer, get_id);
  EXPECT_CALL(layer, get_type);
  EXPECT_CALL(layer, get_opacity);
  EXPECT_CALL(layer, is_visible);
  EXPECT_CALL(layer, layer_count).Times(0);
  EXPECT_CALL(layer, object_count).Times(0);
  EXPECT_CALL(layer, get_tile).Times(9);
  EXPECT_CALL(layer, get_tile_encoding);
  EXPECT_CALL(layer, get_extent);
  EXPECT_CALL(layer, get_meta);
  EXPECT_CALL(layer.get_meta_view_mock(), get_name);

  const auto layer_json = emit_tiled_tmj_layer(mRuntime, layer, mTileByteStream);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("id"), ir_layer.id);
  EXPECT_EQ(layer_json->at("name"), ir_layer.meta.name);
  EXPECT_EQ(layer_json->at("type"), "tilelayer");
  EXPECT_EQ(layer_json->at("opacity"), ir_layer.opacity);
  EXPECT_EQ(layer_json->at("visible"), ir_layer.visible);
  EXPECT_EQ(layer_json->at("x"), 0);
  EXPECT_EQ(layer_json->at("y"), 0);
  EXPECT_EQ(layer_json->at("width"), ir_layer.extent.cols);
  EXPECT_EQ(layer_json->at("height"), ir_layer.extent.rows);

  EXPECT_TRUE(layer_json->at("data").is_array());
  EXPECT_FALSE(layer_json->contains("encoding"));
}

// tactile::emit_tiled_tmj_layer
TEST_F(TmjFormatLayerEmitterTest, EmitBase64TileLayer)
{
  const ir::Layer ir_layer {
    .meta =
        ir::Metadata {
          .name = "b64",
          .properties = {},
          .components = {},
        },
    .id = LayerID {42},
    .type = LayerType::kTileLayer,
    .opacity = 0.5f,
    .extent = Extent2D {4, 5},
    .tiles =
        TileMatrix {
          {TileID {123}, TileID {123}, TileID {123}, TileID {123}, TileID {123}},
          {TileID {123}, TileID {123}, TileID {123}, TileID {123}, TileID {123}},
          {TileID {123}, TileID {123}, TileID {123}, TileID {123}, TileID {123}},
          {TileID {123}, TileID {123}, TileID {123}, TileID {123}, TileID {123}},
        },
    .objects = {},
    .layers = {},
    .visible = true,
  };

  const ir::TileFormat ir_tile_format {
    .encoding = TileEncoding::kBase64,
    .compression = std::nullopt,
    .compression_level = std::nullopt,
  };

  testing::NiceMock<LayerViewMock> layer {ir_layer, ir_tile_format};
  EXPECT_CALL(layer, write_tile_bytes);
  EXPECT_CALL(layer, get_id);
  EXPECT_CALL(layer, get_type);
  EXPECT_CALL(layer, get_opacity);
  EXPECT_CALL(layer, is_visible);
  EXPECT_CALL(layer, layer_count).Times(0);
  EXPECT_CALL(layer, object_count).Times(0);
  EXPECT_CALL(layer, get_tile).Times(0);
  EXPECT_CALL(layer, get_tile_encoding);
  EXPECT_CALL(layer, get_tile_compression);
  EXPECT_CALL(layer, get_extent);
  EXPECT_CALL(layer, get_meta);
  EXPECT_CALL(layer.get_meta_view_mock(), get_name);

  const auto layer_json = emit_tiled_tmj_layer(mRuntime, layer, mTileByteStream);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("id"), ir_layer.id);
  EXPECT_EQ(layer_json->at("name"), ir_layer.meta.name);
  EXPECT_EQ(layer_json->at("type"), "tilelayer");
  EXPECT_EQ(layer_json->at("opacity"), ir_layer.opacity);
  EXPECT_EQ(layer_json->at("visible"), ir_layer.visible);
  EXPECT_EQ(layer_json->at("x"), 0);
  EXPECT_EQ(layer_json->at("y"), 0);
  EXPECT_EQ(layer_json->at("width"), ir_layer.extent.cols);
  EXPECT_EQ(layer_json->at("height"), ir_layer.extent.rows);
  EXPECT_EQ(layer_json->at("encoding"), "base64");
  EXPECT_EQ(
      layer_json->at("data"),
      "ewAAAHsAAAB7AAAAewAAAHsAAAB7AAAAewAAAHsAAAB7AAAAewAAAHsAAAB7AAAAewAAAHsAAAB7AAAAewAAAHsAAAB7AAAAewAAAHsAAAA=");
}

#if TACTILE_HAS_ZLIB_COMPRESSION

// tactile::emit_tiled_tmj_layer
TEST_F(TmjFormatLayerEmitterTest, EmitZlibCompressedTileLayer)
{
  const ir::Layer ir_layer {
    .meta =
        ir::Metadata {
          .name = "zlib_layer",
          .properties = {},
          .components = {},
        },
    .id = LayerID {824},
    .type = LayerType::kTileLayer,
    .opacity = 1.0f,
    .extent = Extent2D {3, 3},
    .tiles =
        TileMatrix {
          {TileID {1}, TileID {2}, TileID {3}},
          {TileID {4}, TileID {5}, TileID {6}},
        },
    .objects = {},
    .layers = {},
    .visible = true,
  };

  const ir::TileFormat ir_tile_format {
    .encoding = TileEncoding::kBase64,
    .compression = CompressionFormat::kZlib,
    .compression_level = std::nullopt,
  };

  testing::NiceMock<LayerViewMock> layer {ir_layer, ir_tile_format};
  EXPECT_CALL(layer, write_tile_bytes);
  EXPECT_CALL(layer, get_id);
  EXPECT_CALL(layer, get_type);
  EXPECT_CALL(layer, get_opacity);
  EXPECT_CALL(layer, is_visible);
  EXPECT_CALL(layer, layer_count).Times(0);
  EXPECT_CALL(layer, object_count).Times(0);
  EXPECT_CALL(layer, get_tile).Times(0);
  EXPECT_CALL(layer, get_tile_encoding);
  EXPECT_CALL(layer, get_tile_compression);
  EXPECT_CALL(layer, get_extent);
  EXPECT_CALL(layer, get_meta);
  EXPECT_CALL(layer.get_meta_view_mock(), get_name);

  const auto layer_json = emit_tiled_tmj_layer(mRuntime, layer, mTileByteStream);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("id"), ir_layer.id);
  EXPECT_EQ(layer_json->at("name"), ir_layer.meta.name);
  EXPECT_EQ(layer_json->at("type"), "tilelayer");
  EXPECT_EQ(layer_json->at("opacity"), ir_layer.opacity);
  EXPECT_EQ(layer_json->at("visible"), ir_layer.visible);
  EXPECT_EQ(layer_json->at("x"), 0);
  EXPECT_EQ(layer_json->at("y"), 0);
  EXPECT_EQ(layer_json->at("width"), ir_layer.extent.cols);
  EXPECT_EQ(layer_json->at("height"), ir_layer.extent.rows);
  EXPECT_EQ(layer_json->at("encoding"), "base64");
  EXPECT_EQ(layer_json->at("compression"), "zlib");
  EXPECT_EQ(layer_json->at("data"), "eJxjZGBgYAJiZiBmAWJWIGYDYgAA+AAW");
}

#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#if TACTILE_HAS_ZSTD_COMPRESSION

// tactile::emit_tiled_tmj_layer
TEST_F(TmjFormatLayerEmitterTest, EmitZstdCompressedTileLayer)
{
  const ir::Layer ir_layer {
    .meta =
        ir::Metadata {
          .name = "zstd_layer",
          .properties = {},
          .components = {},
        },
    .id = LayerID {573},
    .type = LayerType::kTileLayer,
    .opacity = 1.0f,
    .extent = Extent2D {3, 3},
    .tiles =
        TileMatrix {
          {TileID {1}, TileID {2}, TileID {3}},
          {TileID {4}, TileID {5}, TileID {6}},
        },
    .objects = {},
    .layers = {},
    .visible = true,
  };

  const ir::TileFormat ir_tile_format {
    .encoding = TileEncoding::kBase64,
    .compression = CompressionFormat::kZstd,
    .compression_level = std::nullopt,
  };

  testing::NiceMock<LayerViewMock> layer {ir_layer, ir_tile_format};
  EXPECT_CALL(layer, write_tile_bytes);
  EXPECT_CALL(layer, get_id);
  EXPECT_CALL(layer, get_type);
  EXPECT_CALL(layer, get_opacity);
  EXPECT_CALL(layer, is_visible);
  EXPECT_CALL(layer, layer_count).Times(0);
  EXPECT_CALL(layer, object_count).Times(0);
  EXPECT_CALL(layer, get_tile).Times(0);
  EXPECT_CALL(layer, get_tile_encoding);
  EXPECT_CALL(layer, get_tile_compression);
  EXPECT_CALL(layer, get_extent);
  EXPECT_CALL(layer, get_meta);
  EXPECT_CALL(layer.get_meta_view_mock(), get_name);

  const auto layer_json = emit_tiled_tmj_layer(mRuntime, layer, mTileByteStream);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("id"), ir_layer.id);
  EXPECT_EQ(layer_json->at("name"), ir_layer.meta.name);
  EXPECT_EQ(layer_json->at("type"), "tilelayer");
  EXPECT_EQ(layer_json->at("opacity"), ir_layer.opacity);
  EXPECT_EQ(layer_json->at("visible"), ir_layer.visible);
  EXPECT_EQ(layer_json->at("x"), 0);
  EXPECT_EQ(layer_json->at("y"), 0);
  EXPECT_EQ(layer_json->at("width"), ir_layer.extent.cols);
  EXPECT_EQ(layer_json->at("height"), ir_layer.extent.rows);
  EXPECT_EQ(layer_json->at("encoding"), "base64");
  EXPECT_EQ(layer_json->at("compression"), "zstd");
  EXPECT_EQ(layer_json->at("data"), "KLUv/SAYwQAAAQAAAAIAAAADAAAABAAAAAUAAAAGAAAA");
}

#endif  // TACTILE_HAS_ZSTD_COMPRESSION

// tactile::emit_tiled_tmj_layer
TEST_F(TmjFormatLayerEmitterTest, EmitObjectLayer)
{
  const ir::Layer ir_layer {
    .meta =
        ir::Metadata {
          .name = "objects",
          .properties = {},
          .components = {},
        },
    .id = LayerID {1},
    .type = LayerType::kObjectLayer,
    .opacity = 1.0f,
    .extent = Extent2D {0, 0},
    .tiles = {},
    .objects =
        {
          make_ir_object(ObjectID {1}),
          make_ir_object(ObjectID {2}),
          make_ir_object(ObjectID {3}),
        },
    .layers = {},
    .visible = false,
  };

  const ir::TileFormat ir_tile_format {
    .encoding = TileEncoding::kPlainText,
    .compression = std::nullopt,
    .compression_level = std::nullopt,
  };

  testing::NiceMock<LayerViewMock> layer {ir_layer, ir_tile_format};
  EXPECT_CALL(layer, write_tile_bytes).Times(0);
  EXPECT_CALL(layer, get_id);
  EXPECT_CALL(layer, get_type);
  EXPECT_CALL(layer, get_opacity);
  EXPECT_CALL(layer, is_visible);
  EXPECT_CALL(layer, layer_count).Times(0);
  EXPECT_CALL(layer, object_count);
  EXPECT_CALL(layer, get_tile).Times(0);
  EXPECT_CALL(layer, get_tile_encoding).Times(0);
  EXPECT_CALL(layer, get_tile_compression).Times(0);
  EXPECT_CALL(layer, get_extent).Times(0);
  EXPECT_CALL(layer, get_meta);
  EXPECT_CALL(layer.get_meta_view_mock(), get_name);

  const auto layer_json = emit_tiled_tmj_layer(mRuntime, layer, mTileByteStream);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("id"), ir_layer.id);
  EXPECT_EQ(layer_json->at("name"), ir_layer.meta.name);
  EXPECT_EQ(layer_json->at("type"), "objectgroup");
  EXPECT_EQ(layer_json->at("opacity"), ir_layer.opacity);
  EXPECT_EQ(layer_json->at("visible"), ir_layer.visible);
  EXPECT_EQ(layer_json->at("x"), 0);
  EXPECT_EQ(layer_json->at("y"), 0);

  ASSERT_TRUE(layer_json->contains("objects"));
  EXPECT_EQ(layer_json->at("objects").get_ref<const nlohmann::json::array_t&>().capacity(),
            ir_layer.objects.size());
}

// tactile::emit_tiled_tmj_layer
TEST_F(TmjFormatLayerEmitterTest, EmitGroupLayer)
{
  const ir::Layer ir_layer {
    .meta =
        ir::Metadata {
          .name = "group",
          .properties = {},
          .components = {},
        },
    .id = LayerID {58},
    .type = LayerType::kGroupLayer,
    .opacity = 0.2f,
    .extent = Extent2D {0, 0},
    .tiles = {},
    .objects = {},
    .layers =
        {
          make_ir_object_layer(LayerID {59}),
          make_ir_object_layer(LayerID {60}),
          make_ir_object_layer(LayerID {61}),
          make_ir_object_layer(LayerID {62}),
          make_ir_object_layer(LayerID {63}),
        },
    .visible = true,
  };

  const ir::TileFormat ir_tile_format {
    .encoding = TileEncoding::kPlainText,
    .compression = std::nullopt,
    .compression_level = std::nullopt,
  };

  testing::NiceMock<LayerViewMock> layer {ir_layer, ir_tile_format};
  EXPECT_CALL(layer, write_tile_bytes).Times(0);
  EXPECT_CALL(layer, get_id);
  EXPECT_CALL(layer, get_type);
  EXPECT_CALL(layer, get_opacity);
  EXPECT_CALL(layer, is_visible);
  EXPECT_CALL(layer, layer_count);
  EXPECT_CALL(layer, object_count).Times(0);
  EXPECT_CALL(layer, get_tile).Times(0);
  EXPECT_CALL(layer, get_tile_encoding).Times(0);
  EXPECT_CALL(layer, get_tile_compression).Times(0);
  EXPECT_CALL(layer, get_extent).Times(0);
  EXPECT_CALL(layer, get_meta);
  EXPECT_CALL(layer.get_meta_view_mock(), get_name);

  const auto layer_json = emit_tiled_tmj_layer(mRuntime, layer, mTileByteStream);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("id"), ir_layer.id);
  EXPECT_EQ(layer_json->at("name"), ir_layer.meta.name);
  EXPECT_EQ(layer_json->at("type"), "group");
  EXPECT_EQ(layer_json->at("opacity"), ir_layer.opacity);
  EXPECT_EQ(layer_json->at("visible"), ir_layer.visible);
  EXPECT_EQ(layer_json->at("x"), 0);
  EXPECT_EQ(layer_json->at("y"), 0);

  ASSERT_TRUE(layer_json->contains("layers"));
  EXPECT_EQ(layer_json->at("layers").get_ref<const nlohmann::json::array_t&>().capacity(),
            ir_layer.layers.size());
}

}  // namespace tactile::test
