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

  // clang-format off
  auto& tile_layer = map.layers.emplace_back();
  tile_layer.width = map.col_count;
  tile_layer.height = map.row_count;
  tile_layer.tiles = TileMatrix {
    TileRow {TileID{0x00}, TileID{0x01}, TileID{0x02}, TileID{0x03}, TileID{0x04}, TileID{0x05}, TileID{0x06}, TileID{0x07}, TileID{0x08}, TileID{0x09}},
    TileRow {TileID{0x10}, TileID{0x11}, TileID{0x12}, TileID{0x13}, TileID{0x14}, TileID{0x15}, TileID{0x16}, TileID{0x17}, TileID{0x18}, TileID{0x19}},
    TileRow {TileID{0x20}, TileID{0x21}, TileID{0x22}, TileID{0x23}, TileID{0x24}, TileID{0x25}, TileID{0x26}, TileID{0x27}, TileID{0x28}, TileID{0x29}},
    TileRow {TileID{0x30}, TileID{0x31}, TileID{0x32}, TileID{0x33}, TileID{0x34}, TileID{0x35}, TileID{0x36}, TileID{0x37}, TileID{0x38}, TileID{0x39}},
    TileRow {TileID{0x40}, TileID{0x41}, TileID{0x42}, TileID{0x43}, TileID{0x44}, TileID{0x45}, TileID{0x46}, TileID{0x47}, TileID{0x48}, TileID{0x49}},
    TileRow {TileID{0x50}, TileID{0x51}, TileID{0x52}, TileID{0x53}, TileID{0x54}, TileID{0x55}, TileID{0x56}, TileID{0x57}, TileID{0x58}, TileID{0x59}},
    TileRow {TileID{0x60}, TileID{0x61}, TileID{0x62}, TileID{0x63}, TileID{0x64}, TileID{0x65}, TileID{0x66}, TileID{0x67}, TileID{0x68}, TileID{0x69}},
    TileRow {TileID{0x70}, TileID{0x71}, TileID{0x72}, TileID{0x73}, TileID{0x74}, TileID{0x75}, TileID{0x76}, TileID{0x77}, TileID{0x78}, TileID{0x79}},
  };
  // clang-format on

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
