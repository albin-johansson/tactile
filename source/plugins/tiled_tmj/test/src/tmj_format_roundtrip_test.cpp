// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <filesystem>   // current_path
#include <optional>     // optional
#include <ostream>      // ostream
#include <string_view>  // string_view

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/runtime/command_line_options.hpp"
#include "tactile/runtime/runtime.hpp"
#include "tactile/test_util/document_view_mocks.hpp"
#include "tactile/test_util/ir.hpp"
#include "tactile/test_util/ir_eq.hpp"
#include "tactile/test_util/ir_presets.hpp"
#include "tactile/tiled_tmj/tmj_format_plugin.hpp"

#ifdef TACTILE_HAS_ZLIB_COMPRESSION
  #include "tactile/zlib_compression/zlib_compression_plugin.hpp"
#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#ifdef TACTILE_HAS_ZSTD_COMPRESSION
  #include "tactile/zstd_compression/zstd_compression_plugin.hpp"
#endif  // TACTILE_HAS_ZSTD_COMPRESSION

namespace tactile::test {

struct TmjRoundtripConfig final
{
  std::string_view map_filename;
  TileEncoding encoding;
  std::optional<CompressionFormat> compression;
  bool use_external_tilesets;
};

inline auto operator<<(std::ostream& stream, const TmjRoundtripConfig& config) -> std::ostream&
{
  stream << config.map_filename;

  stream << " + " << (config.encoding == TileEncoding::kBase64 ? "base64" : "plain text")
         << " encoding";

  if (config.compression == CompressionFormat::kZlib) {
    stream << " + zlib compression";
  }
  else if (config.compression == CompressionFormat::kZstd) {
    stream << " + zstd compression";
  }
  else {
    stream << " + no compression";
  }

  stream << " + " << (config.use_external_tilesets ? "external" : "embedded") << " tilesets";

  return stream;
}

class TmjFormatRoundtripTest : public testing::TestWithParam<TmjRoundtripConfig>
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
  Runtime mRuntime {get_default_command_line_options()};

#ifdef TACTILE_HAS_ZLIB_COMPRESSION
  ZlibCompressionPlugin mZlibCompressionPlugin {};
#endif  // TACTILE_HAS_ZLIB_COMPRESSION

#ifdef TACTILE_HAS_ZSTD_COMPRESSION
  ZstdCompressionPlugin mZstdCompressionPlugin {};
#endif  // TACTILE_HAS_ZSTD_COMPRESSION

  TmjFormatPlugin mTmjFormatPlugin {};
};

INSTANTIATE_TEST_SUITE_P(TMJ,
                         TmjFormatRoundtripTest,
                         testing::Values(
                             TmjRoundtripConfig {
                               .map_filename = "map_with_embedded_tilesets.tmj",
                               .encoding = TileEncoding::kPlainText,
                               .compression = std::nullopt,
                               .use_external_tilesets = false,
                             },
                             TmjRoundtripConfig {
                               .map_filename = "map_with_external_tilesets.tmj",
                               .encoding = TileEncoding::kPlainText,
                               .compression = std::nullopt,
                               .use_external_tilesets = true,
                             },
                             TmjRoundtripConfig {
                               .map_filename = "map_with_base64_tiles.tmj",
                               .encoding = TileEncoding::kBase64,
                               .compression = std::nullopt,
                               .use_external_tilesets = false,
                             },
                             TmjRoundtripConfig {
                               .map_filename = "map_with_base64_zlib_tiles.tmj",
                               .encoding = TileEncoding::kBase64,
                               .compression = CompressionFormat::kZlib,
                               .use_external_tilesets = false,
                             },
                             TmjRoundtripConfig {
                               .map_filename = "map_with_base64_zstd_tiles.tmj",
                               .encoding = TileEncoding::kBase64,
                               .compression = CompressionFormat::kZstd,
                               .use_external_tilesets = false,
                             }));

TEST_P(TmjFormatRoundtripTest, SaveAndLoadMap)
{
  const auto& config = GetParam();

  const auto* save_format = mRuntime.get_save_format(SaveFormatId::kTiledTmj);
  ASSERT_NE(save_format, nullptr);

  auto ir_map = make_complex_ir_map(ir::TileFormat {
    .encoding = config.encoding,
    .compression = config.compression,
    .compression_level = std::nullopt,
  });

  for (auto& ir_tileset_ref : ir_map.tilesets) {
    ir_tileset_ref.tileset.is_embedded = !config.use_external_tilesets;
  }

  const testing::NiceMock<MapViewMock> map_view {ir_map};

  const auto map_path = std::filesystem::current_path() / config.map_filename;
  EXPECT_CALL(map_view, get_path).WillRepeatedly(testing::Return(&map_path));

  const SaveFormatWriteOptions write_options {
    .base_dir = map_path.parent_path(),
    .use_external_tilesets = config.use_external_tilesets,
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
