// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_layer_emitter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/base/io/tile_io.hpp"
#include "tactile/base/platform/bits.hpp"
#include "tactile/base/test_util/document_view_mocks.hpp"
#include "tactile/runtime/runtime.hpp"

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
    mZlibPlugin.load(mRuntime);
#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#ifdef TACTILE_HAS_ZSTD_COMPRESSION
    mZstdPlugin.load(mRuntime);
#endif  // TACTILE_HAS_ZSTD_COMPRESSION
  }

  void TearDown() override
  {
#ifdef TACTILE_HAS_ZLIB_COMPRESSION
    mZlibPlugin.unload(mRuntime);
#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#ifdef TACTILE_HAS_ZSTD_COMPRESSION
    mZstdPlugin.unload(mRuntime);
#endif  // TACTILE_HAS_ZSTD_COMPRESSION
  }

 protected:
  Runtime mRuntime {};

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
  using testing::Return;

  const StringView name {"plain"};
  const LayerID id {34};
  const MatrixExtent extent {3, 3};
  const TileID tile_id {123};
  const float opacity {0.1f};
  const bool visible {false};
  const TileEncoding encoding {TileEncoding::kPlainText};

  testing::NiceMock<LayerViewMock> layer {};
  EXPECT_CALL(layer, write_tile_bytes).Times(0);
  EXPECT_CALL(layer, get_id).WillRepeatedly(Return(id));
  EXPECT_CALL(layer, get_type).WillRepeatedly(Return(LayerType::kTileLayer));
  EXPECT_CALL(layer, get_opacity).WillRepeatedly(Return(opacity));
  EXPECT_CALL(layer, is_visible).WillRepeatedly(Return(visible));
  EXPECT_CALL(layer, layer_count).Times(0);
  EXPECT_CALL(layer, object_count).Times(0);
  EXPECT_CALL(layer, get_tile).WillRepeatedly(Return(tile_id));
  EXPECT_CALL(layer, get_tile_encoding).WillRepeatedly(Return(encoding));
  EXPECT_CALL(layer, get_extent).WillRepeatedly(Return(extent));
  EXPECT_CALL(layer, get_meta);
  EXPECT_CALL(layer.get_meta_view_mock(), get_name).WillRepeatedly(Return(name));

  const auto layer_json = emit_tiled_tmj_layer(mRuntime, layer, mTileByteStream);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("id"), id);
  EXPECT_EQ(layer_json->at("name"), name);
  EXPECT_EQ(layer_json->at("type"), "tilelayer");
  EXPECT_EQ(layer_json->at("opacity"), opacity);
  EXPECT_EQ(layer_json->at("visible"), visible);
  EXPECT_EQ(layer_json->at("x"), 0);
  EXPECT_EQ(layer_json->at("y"), 0);
  EXPECT_EQ(layer_json->at("width"), extent.cols);
  EXPECT_EQ(layer_json->at("height"), extent.rows);

  EXPECT_TRUE(layer_json->at("data").is_array());
  EXPECT_FALSE(layer_json->contains("encoding"));
}

// tactile::emit_tiled_tmj_layer
TEST_F(TmjFormatLayerEmitterTest, EmitBase64TileLayer)
{
  using testing::Return;

  const StringView name {"b64"};
  const LayerID id {42};
  const MatrixExtent extent {4, 5};
  const TileID tile_id {123};
  const float opacity {0.5f};
  const bool visible {true};
  const TileEncoding encoding {TileEncoding::kBase64};

  testing::NiceMock<LayerViewMock> layer {};
  EXPECT_CALL(layer, write_tile_bytes).WillRepeatedly([](ByteStream& byte_stream) {
    each_byte(to_little_endian(tile_id),
              [&](const uint8 byte) { byte_stream.push_back(byte); });
  });
  EXPECT_CALL(layer, get_id).WillRepeatedly(Return(id));
  EXPECT_CALL(layer, get_type).WillRepeatedly(Return(LayerType::kTileLayer));
  EXPECT_CALL(layer, get_opacity).WillRepeatedly(Return(opacity));
  EXPECT_CALL(layer, is_visible).WillRepeatedly(Return(visible));
  EXPECT_CALL(layer, layer_count).Times(0);
  EXPECT_CALL(layer, object_count).Times(0);
  EXPECT_CALL(layer, get_tile).WillRepeatedly(Return(tile_id));
  EXPECT_CALL(layer, get_tile_encoding).WillRepeatedly(Return(encoding));
  EXPECT_CALL(layer, get_tile_compression);
  EXPECT_CALL(layer, get_extent).WillRepeatedly(Return(extent));
  EXPECT_CALL(layer, get_meta);
  EXPECT_CALL(layer.get_meta_view_mock(), get_name).WillRepeatedly(Return(name));

  const auto layer_json = emit_tiled_tmj_layer(mRuntime, layer, mTileByteStream);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("id"), id);
  EXPECT_EQ(layer_json->at("name"), name);
  EXPECT_EQ(layer_json->at("type"), "tilelayer");
  EXPECT_EQ(layer_json->at("opacity"), opacity);
  EXPECT_EQ(layer_json->at("visible"), visible);
  EXPECT_EQ(layer_json->at("x"), 0);
  EXPECT_EQ(layer_json->at("y"), 0);
  EXPECT_EQ(layer_json->at("width"), extent.cols);
  EXPECT_EQ(layer_json->at("height"), extent.rows);
  EXPECT_EQ(layer_json->at("encoding"), "base64");
  EXPECT_EQ(layer_json->at("data"), "ewAAAA==");
}

// tactile::emit_tiled_tmj_layer
TEST_F(TmjFormatLayerEmitterTest, EmitZlibCompressedTileLayer)
{
  using testing::Return;

  const StringView name {"zlib_layer"};
  const LayerID id {824};
  const MatrixExtent extent {4, 5};
  const float opacity {1.0f};
  const bool visible {true};
  const TileEncoding encoding {TileEncoding::kBase64};

  const TileMatrix tile_matrix {
    {TileID {1}, TileID {2}, TileID {3}},
    {TileID {4}, TileID {5}, TileID {6}},
  };

  testing::NiceMock<LayerViewMock> layer {};
  EXPECT_CALL(layer, write_tile_bytes).WillRepeatedly([&](ByteStream& byte_stream) {
    byte_stream = to_byte_stream(tile_matrix);
  });
  EXPECT_CALL(layer, get_id).WillRepeatedly(Return(id));
  EXPECT_CALL(layer, get_type).WillRepeatedly(Return(LayerType::kTileLayer));
  EXPECT_CALL(layer, get_opacity).WillRepeatedly(Return(opacity));
  EXPECT_CALL(layer, is_visible).WillRepeatedly(Return(visible));
  EXPECT_CALL(layer, layer_count).Times(0);
  EXPECT_CALL(layer, object_count).Times(0);
  EXPECT_CALL(layer, get_tile).WillRepeatedly([&](const MatrixIndex& index) {
    return tile_matrix.at(saturate_cast<usize>(index.row)).at(saturate_cast<usize>(index.col));
  });
  EXPECT_CALL(layer, get_tile_encoding).WillRepeatedly(Return(encoding));
  EXPECT_CALL(layer, get_tile_compression).WillRepeatedly(Return(CompressionFormat::kZlib));
  EXPECT_CALL(layer, get_extent).WillRepeatedly(Return(extent));
  EXPECT_CALL(layer, get_meta);
  EXPECT_CALL(layer.get_meta_view_mock(), get_name).WillRepeatedly(Return(name));

  const auto layer_json = emit_tiled_tmj_layer(mRuntime, layer, mTileByteStream);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("id"), id);
  EXPECT_EQ(layer_json->at("name"), name);
  EXPECT_EQ(layer_json->at("type"), "tilelayer");
  EXPECT_EQ(layer_json->at("opacity"), opacity);
  EXPECT_EQ(layer_json->at("visible"), visible);
  EXPECT_EQ(layer_json->at("x"), 0);
  EXPECT_EQ(layer_json->at("y"), 0);
  EXPECT_EQ(layer_json->at("width"), extent.cols);
  EXPECT_EQ(layer_json->at("height"), extent.rows);
  EXPECT_EQ(layer_json->at("encoding"), "base64");
  EXPECT_EQ(layer_json->at("compression"), "zlib");
  EXPECT_EQ(layer_json->at("data"), "eJxjZGBgYAJiZiBmAWJWIGYDYgAA+AAW");
}

// tactile::emit_tiled_tmj_layer
TEST_F(TmjFormatLayerEmitterTest, EmitZstdCompressedTileLayer)
{
  using testing::Return;

  const StringView name {"zstd_layer"};
  const LayerID id {573};
  const MatrixExtent extent {3, 3};
  const float opacity {1.0f};
  const bool visible {true};
  const TileEncoding encoding {TileEncoding::kBase64};

  const TileMatrix tile_matrix {
    {TileID {1}, TileID {2}, TileID {3}},
    {TileID {4}, TileID {5}, TileID {6}},
  };

  testing::NiceMock<LayerViewMock> layer {};
  EXPECT_CALL(layer, write_tile_bytes).WillRepeatedly([&](ByteStream& byte_stream) {
    byte_stream = to_byte_stream(tile_matrix);
  });
  EXPECT_CALL(layer, get_id).WillRepeatedly(Return(id));
  EXPECT_CALL(layer, get_type).WillRepeatedly(Return(LayerType::kTileLayer));
  EXPECT_CALL(layer, get_opacity).WillRepeatedly(Return(opacity));
  EXPECT_CALL(layer, is_visible).WillRepeatedly(Return(visible));
  EXPECT_CALL(layer, layer_count).Times(0);
  EXPECT_CALL(layer, object_count).Times(0);
  EXPECT_CALL(layer, get_tile).WillRepeatedly([&](const MatrixIndex& index) {
    return tile_matrix.at(saturate_cast<usize>(index.row)).at(saturate_cast<usize>(index.col));
  });
  EXPECT_CALL(layer, get_tile_encoding).WillRepeatedly(Return(encoding));
  EXPECT_CALL(layer, get_tile_compression).WillRepeatedly(Return(CompressionFormat::kZstd));
  EXPECT_CALL(layer, get_extent).WillRepeatedly(Return(extent));
  EXPECT_CALL(layer, get_meta);
  EXPECT_CALL(layer.get_meta_view_mock(), get_name).WillRepeatedly(Return(name));

  const auto layer_json = emit_tiled_tmj_layer(mRuntime, layer, mTileByteStream);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("id"), id);
  EXPECT_EQ(layer_json->at("name"), name);
  EXPECT_EQ(layer_json->at("type"), "tilelayer");
  EXPECT_EQ(layer_json->at("opacity"), opacity);
  EXPECT_EQ(layer_json->at("visible"), visible);
  EXPECT_EQ(layer_json->at("x"), 0);
  EXPECT_EQ(layer_json->at("y"), 0);
  EXPECT_EQ(layer_json->at("width"), extent.cols);
  EXPECT_EQ(layer_json->at("height"), extent.rows);
  EXPECT_EQ(layer_json->at("encoding"), "base64");
  EXPECT_EQ(layer_json->at("compression"), "zstd");
  EXPECT_EQ(layer_json->at("data"), "KLUv\/SAYwQAAAQAAAAIAAAADAAAABAAAAAUAAAAGAAAA");
}

// tactile::emit_tiled_tmj_layer
TEST_F(TmjFormatLayerEmitterTest, EmitObjectLayer)
{
  using testing::Return;

  const StringView name {"objects"};
  const LayerID id {1};
  const float opacity {1.0f};
  const bool visible {false};
  const usize object_count {3};

  testing::NiceMock<LayerViewMock> layer {};
  EXPECT_CALL(layer, write_tile_bytes).Times(0);
  EXPECT_CALL(layer, get_id).WillRepeatedly(Return(id));
  EXPECT_CALL(layer, get_type).WillRepeatedly(Return(LayerType::kObjectLayer));
  EXPECT_CALL(layer, get_opacity).WillRepeatedly(Return(opacity));
  EXPECT_CALL(layer, is_visible).WillRepeatedly(Return(visible));
  EXPECT_CALL(layer, layer_count).Times(0);
  EXPECT_CALL(layer, object_count).WillRepeatedly(Return(object_count));
  EXPECT_CALL(layer, get_tile).Times(0);
  EXPECT_CALL(layer, get_tile_encoding).Times(0);
  EXPECT_CALL(layer, get_tile_compression).Times(0);
  EXPECT_CALL(layer, get_extent).Times(0);
  EXPECT_CALL(layer, get_meta);
  EXPECT_CALL(layer.get_meta_view_mock(), get_name).WillRepeatedly(Return(name));

  const auto layer_json = emit_tiled_tmj_layer(mRuntime, layer, mTileByteStream);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("id"), id);
  EXPECT_EQ(layer_json->at("name"), name);
  EXPECT_EQ(layer_json->at("type"), "objectgroup");
  EXPECT_EQ(layer_json->at("opacity"), opacity);
  EXPECT_EQ(layer_json->at("visible"), visible);
  EXPECT_EQ(layer_json->at("x"), 0);
  EXPECT_EQ(layer_json->at("y"), 0);
  ASSERT_TRUE(layer_json->contains("objects"));
  EXPECT_EQ(layer_json->at("objects").get_ref<const nlohmann::json::array_t&>().capacity(),
            object_count);
}

// tactile::emit_tiled_tmj_layer
TEST_F(TmjFormatLayerEmitterTest, EmitGroupLayer)
{
  using testing::Return;

  const StringView name {"group"};
  const LayerID id {58};
  const float opacity {0.2f};
  const bool visible {true};
  const usize layer_count {5};

  testing::NiceMock<LayerViewMock> layer {};
  EXPECT_CALL(layer, write_tile_bytes).Times(0);
  EXPECT_CALL(layer, get_id).WillRepeatedly(Return(id));
  EXPECT_CALL(layer, get_type).WillRepeatedly(Return(LayerType::kGroupLayer));
  EXPECT_CALL(layer, get_opacity).WillRepeatedly(Return(opacity));
  EXPECT_CALL(layer, is_visible).WillRepeatedly(Return(visible));
  EXPECT_CALL(layer, layer_count).WillRepeatedly(Return(layer_count));
  EXPECT_CALL(layer, object_count).Times(0);
  EXPECT_CALL(layer, get_tile).Times(0);
  EXPECT_CALL(layer, get_tile_encoding).Times(0);
  EXPECT_CALL(layer, get_tile_compression).Times(0);
  EXPECT_CALL(layer, get_extent).Times(0);
  EXPECT_CALL(layer, get_meta);
  EXPECT_CALL(layer.get_meta_view_mock(), get_name).WillRepeatedly(Return(name));

  const auto layer_json = emit_tiled_tmj_layer(mRuntime, layer, mTileByteStream);
  ASSERT_TRUE(layer_json.has_value());

  EXPECT_EQ(layer_json->at("id"), id);
  EXPECT_EQ(layer_json->at("name"), name);
  EXPECT_EQ(layer_json->at("type"), "group");
  EXPECT_EQ(layer_json->at("opacity"), opacity);
  EXPECT_EQ(layer_json->at("visible"), visible);
  EXPECT_EQ(layer_json->at("x"), 0);
  EXPECT_EQ(layer_json->at("y"), 0);
  ASSERT_TRUE(layer_json->contains("layers"));
  EXPECT_EQ(layer_json->at("layers").get_ref<const nlohmann::json::array_t&>().capacity(),
            layer_count);
}

}  // namespace tactile::test
