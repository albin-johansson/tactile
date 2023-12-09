// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <utility>  // move

#include <fmt/format.h>
#include <gtest/gtest.h>

#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/io/filesystem.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_manager.hpp"
#include "test/ir_comparison.hpp"

using namespace tactile;
using namespace tactile::fs_literals;

namespace {

inline constexpr usize kMapWidth = 8;
inline constexpr usize kMapHeight = 6;

[[nodiscard]]
auto _make_tile_layer(String name, const int32 id) -> ir::Layer
{
  return {
    .meta =
        {
          .name = std::move(name),
          .properties = {},
          .components = {},
        },
    .id = id,
    .type = LayerType::kTileLayer,
    .opacity = 1.0f,
    .width = kMapWidth,
    .height = kMapHeight,
    .tiles = make_tile_matrix(kMapHeight, kMapWidth),
    .objects = {},
    .layers = {},
    .visible = true,
  };
}

[[nodiscard]]
auto _make_group_layer(String name, const int32 id, Vector<ir::Layer> layers = {})
    -> ir::Layer
{
  return {
    .meta =
        {
          .name = std::move(name),
          .properties = {},
          .components = {},
        },
    .id = id,
    .type = LayerType::kGroupLayer,
    .opacity = 1.0f,
    .width = 0,
    .height = 0,
    .tiles = {},
    .objects = {},
    .layers = std::move(layers),
    .visible = true,
  };
}

}  // namespace

class NestedLayersTest : public testing::TestWithParam<String> {};

INSTANTIATE_TEST_SUITE_P(NestedLayers, NestedLayersTest, testing::Values(".tmj"));

TEST_P(NestedLayersTest, SaveAndLoadMapWithNestedLayers)
{
  const String& file_extension = GetParam();

  const auto map_directory = "test/resource/integration"_path;
  const auto map_filename = fmt::format("nested_layers{}", file_extension);
  const auto map_path = map_directory / map_filename;

  ir::Map map {};
  map.meta.name = map_filename;

  map.row_count = kMapHeight;
  map.col_count = kMapWidth;

  map.layers = {
    _make_tile_layer("T1", map.next_layer_id++),
    _make_group_layer(
        "G1",
        map.next_layer_id++,
        {
          _make_tile_layer("T2", map.next_layer_id++),
          _make_group_layer(
              "G2",
              map.next_layer_id++,
              {
                _make_tile_layer("T3", map.next_layer_id++),
                _make_group_layer("G3",
                                  map.next_layer_id++,
                                  {
                                    _make_tile_layer("T4", map.next_layer_id++),
                                  }),
              }),
          _make_tile_layer("T5", map.next_layer_id++),
        }),
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
