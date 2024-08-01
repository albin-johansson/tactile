// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <filesystem>  // current_path

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/base/test_util/document_view_mocks.hpp"
#include "tactile/base/test_util/ir.hpp"
#include "tactile/base/test_util/ir_eq.hpp"
#include "tactile/base/test_util/ir_presets.hpp"
#include "tactile/runtime/runtime.hpp"
#include "tactile/tiled_tmj_format/tmj_format_plugin.hpp"

#ifdef TACTILE_HAS_ZLIB_COMPRESSION
  #include "tactile/zlib_compression/zlib_compression_plugin.hpp"
#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#ifdef TACTILE_HAS_ZSTD_COMPRESSION
  #include "tactile/zstd_compression/zstd_compression_plugin.hpp"
#endif  // TACTILE_HAS_ZSTD_COMPRESSION

namespace tactile::test {

class TmjFormatRoundtripTest : public testing::Test
{
 public:
  void SetUp() override
  {
#ifdef TACTILE_HAS_ZLIB_COMPRESSION
    mZlibCompressionPlugin.load(&mRuntime);
#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#ifdef TACTILE_HAS_ZSTD_COMPRESSION
    mZstdCompressionPlugin.load(&mRuntime);
#endif  // TACTILE_HAS_ZSTD_COMPRESSION

    mTmjFormatPlugin.load(&mRuntime);
  }

  void TearDown() override
  {
    mTmjFormatPlugin.unload();

#ifdef TACTILE_HAS_ZSTD_COMPRESSION
    mZstdCompressionPlugin.unload();
#endif  // TACTILE_HAS_ZSTD_COMPRESSION

#ifdef TACTILE_HAS_ZLIB_COMPRESSION
    mZlibCompressionPlugin.unload();
#endif  // TACTILE_HAS_ZLIB_COMPRESSION
  }

 protected:
  Runtime mRuntime {};

#ifdef TACTILE_HAS_ZLIB_COMPRESSION
  ZlibCompressionPlugin mZlibCompressionPlugin {};
#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#ifdef TACTILE_HAS_ZSTD_COMPRESSION
  ZstdCompressionPlugin mZstdCompressionPlugin {};
#endif  // TACTILE_HAS_ZSTD_COMPRESSION

  TmjFormatPlugin mTmjFormatPlugin {};
};

TEST_F(TmjFormatRoundtripTest, SaveAndLoadMapWithEmbeddedTilesets)
{
  const auto* save_format = mRuntime.get_save_format(SaveFormatId::kTiledTmj);
  ASSERT_NE(save_format, nullptr);

  auto ir_map = make_complex_ir_map(ir::TileFormat {
    .encoding = TileEncoding::kPlainText,
    .compression = kNone,
    .compression_level = kNone,
  });

  for (auto& ir_tileset_ref : ir_map.tilesets) {
    ir_tileset_ref.tileset.is_embedded = true;
  }

  const testing::NiceMock<MapViewMock> map_view {ir_map};

  const auto map_path = std::filesystem::current_path() / "roundtrip.tmj";
  EXPECT_CALL(map_view, get_path).WillRepeatedly(testing::Return(&map_path));

  const SaveFormatWriteOptions write_options {
    .base_dir = map_path.parent_path(),
    .use_external_tilesets = false,
    .use_indentation = true,
    .fold_tile_layer_data = false,
  };

  ASSERT_TRUE(save_format->save_map(map_view, write_options).has_value());

  const SaveFormatReadOptions read_options {
    .base_dir = write_options.base_dir,
    .strict_mode = false,
  };

  const auto parsed_map = save_format->load_map(map_path, read_options);
  ASSERT_TRUE(parsed_map.has_value());

  expect_eq(ir_map, *parsed_map, AttributeEqMode::kTiled);
}

}  // namespace tactile::test
