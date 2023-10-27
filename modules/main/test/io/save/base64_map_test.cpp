// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <ostream>  // ostream

#include <fmt/format.h>
#include <gtest/gtest.h>

#include "tactile/core/container/string.hpp"
#include "tactile/core/debug/error.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/io/save/save_format_manager.hpp"
#include "test/ir_comparison.hpp"

using namespace tactile;
using namespace tactile::fs_literals;

namespace {

struct Base64MapTestData final {
  String file_extension;
  CompressionMode compression_mode;
};

[[nodiscard]]
auto _get_test_name(const CompressionMode compression_mode) -> String
{
  switch (compression_mode) {
    case CompressionMode::kNone: return "plain";
    case CompressionMode::kZlib: return "zlib";
    case CompressionMode::kZstd: return "zstd";
  }

  throw Error {"Invalid compression mode"};
}

auto operator<<(std::ostream& stream, const Base64MapTestData& data) -> std::ostream&
{
  stream << '\'' << data.file_extension << "', " << _get_test_name(data.compression_mode);
  return stream;
}

}  // namespace

class Base64MapTest : public testing::TestWithParam<Base64MapTestData> {};

INSTANTIATE_TEST_SUITE_P(
    Base64,
    Base64MapTest,
    testing::Values(Base64MapTestData {".tmj", CompressionMode::kNone},
                    Base64MapTestData {".tmj", CompressionMode::kZlib},
                    Base64MapTestData {".tmj", CompressionMode::kZstd}));

TEST_P(Base64MapTest, SaveAndLoadBase64TileLayer)
{
  const auto& test_data = GetParam();

  const auto map_directory = "test/resource/integration"_path;
  const auto map_filename = fmt::format("base64_map_{}{}",
                                        _get_test_name(test_data.compression_mode),
                                        test_data.file_extension);
  const auto map_path = map_directory / map_filename;

  ir::Map map {};
  map.meta.name = map_filename;

  map.row_count = 8;
  map.col_count = 10;

  map.tile_format.encoding = TileEncoding::kBase64;
  map.tile_format.compression = test_data.compression_mode;

  auto& tile_layer = map.layers.emplace_back();
  tile_layer.width = map.col_count;
  tile_layer.height = map.row_count;
  tile_layer.tiles = TileMatrix {
    TileRow {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09},
    TileRow {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19},
    TileRow {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29},
    TileRow {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39},
    TileRow {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49},
    TileRow {0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59},
    TileRow {0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69},
    TileRow {0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79},
  };

  const SaveFormatWriteOptions write_options = {
    .base_dir = map_directory,
    .use_external_tilesets = false,
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
