// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <ostream>  // ostream
#include <utility>  // move

#include <fmt/format.h>
#include <gtest/gtest.h>

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/io/filesystem.hpp"
#include "tactile/foundation/io/save/save_format_context.hpp"
#include "tactile/testutil/ir/ir_comparison.hpp"
#include "tactile/testutil/ir/layers.hpp"
#include "tactile/testutil/ir/maps.hpp"

using namespace tactile;

using fs_literals::operator""_path;

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

  throw Exception {"Invalid compression mode"};
}

auto operator<<(std::ostream& stream, const Base64MapTestData& data) -> std::ostream&
{
  stream << '\'' << data.file_extension << "', " << _get_test_name(data.compression_mode);
  return stream;
}

}  // namespace

class Base64MapTest : public testing::TestWithParam<Base64MapTestData> {};

// NOLINTBEGIN(*-trailing-return-type)

// NOLINTBEGIN(*-use-anonymous-namespace)
INSTANTIATE_TEST_SUITE_P(
    Base64,
    Base64MapTest,
    testing::Values(Base64MapTestData {".yml", CompressionMode::kNone},
                    Base64MapTestData {".yml", CompressionMode::kZlib},
                    Base64MapTestData {".yml", CompressionMode::kZstd},
                    Base64MapTestData {".tmj", CompressionMode::kNone},
                    Base64MapTestData {".tmj", CompressionMode::kZlib},
                    Base64MapTestData {".tmj", CompressionMode::kZstd},
                    Base64MapTestData {".tmx", CompressionMode::kNone},
                    Base64MapTestData {".tmx", CompressionMode::kZlib},
                    Base64MapTestData {".tmx", CompressionMode::kZstd}));
// NOLINTEND(*-use-anonymous-namespace)

TEST_P(Base64MapTest, SaveAndLoadBase64TileLayer)
{
  const auto& test_data = GetParam();

  const auto map_directory = "assets/test/integration/base64"_path;
  const auto map_filename = fmt::format("base64_map_{}{}",
                                        _get_test_name(test_data.compression_mode),
                                        test_data.file_extension);
  const auto map_path = map_directory / map_filename;

  const MatrixExtent map_extent {8, 10};
  auto map = testutil::make_ir_map(map_filename, map_extent);

  map.tile_format.encoding = TileEncoding::kBase64;
  map.tile_format.compression = test_data.compression_mode;

  auto layer = testutil::make_ir_tile_layer(map.next_layer_id++, map_extent);
  layer.tiles = testutil::make_tile_matrix_with_increasing_tiles(map_extent, 42);

  map.layers.push_back(std::move(layer));

  const SaveFormatWriteOptions write_options = {
    .base_dir = map_directory,
    .use_external_tilesets = false,
    .use_indentation = true,
    .fold_tile_layer_data = true,
  };

  const SaveFormatReadOptions read_options = {
    .base_dir = map_directory,
    .strict_mode = false,
  };

  const auto& save_format_context = SaveFormatContext::get();
  ASSERT_TRUE(save_format_context.save_map(map_path, map, write_options).has_value());

  const auto parsed_map = save_format_context.load_map(map_path, read_options);
  ASSERT_TRUE(parsed_map.has_value());

  testutil::expect_eq(*parsed_map, map);
}

// NOLINTEND(*-trailing-return-type)
