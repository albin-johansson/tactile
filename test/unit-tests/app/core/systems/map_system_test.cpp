#include "core/systems/map_system.hpp"

#include <gtest/gtest.h>

#include "core/common/ecs.hpp"
#include "core/common/functional.hpp"
#include "core/common/random.hpp"
#include "core/components/map_info.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/registry_system.hpp"
#include "core/tile_pos.hpp"

using namespace tactile;

TEST(MapSystem, AddRow)
{
  auto registry = sys::new_map_document_registry();
  auto& map = ctx_get<comp::MapInfo>(registry);
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  sys::add_row_to_map(registry);
  ASSERT_EQ(6u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  sys::add_row_to_map(registry);
  sys::add_row_to_map(registry);
  ASSERT_EQ(8u, map.row_count);
  ASSERT_EQ(5u, map.column_count);
}

TEST(MapSystem, AddColumn)
{
  auto registry = sys::new_map_document_registry();
  auto& map = ctx_get<comp::MapInfo>(registry);
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  sys::add_column_to_map(registry);
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(6u, map.column_count);

  sys::add_column_to_map(registry);
  sys::add_column_to_map(registry);
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(8u, map.column_count);
}

TEST(MapSystem, RemoveRow)
{
  auto registry = sys::new_map_document_registry();
  auto& map = ctx_get<comp::MapInfo>(registry);
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  sys::remove_row_from_map(registry);
  ASSERT_EQ(4u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  invoke_n(4, [&] { sys::remove_row_from_map(registry); });
  ASSERT_EQ(1u, map.row_count);
  ASSERT_EQ(5u, map.column_count);
}

TEST(MapSystem, RemoveColumn)
{
  auto registry = sys::new_map_document_registry();
  auto& map = ctx_get<comp::MapInfo>(registry);
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  sys::remove_column_from_map(registry);
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(4u, map.column_count);

  invoke_n(4, [&] { sys::remove_column_from_map(registry); });
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(1u, map.column_count);
}

TEST(MapSystem, ResizeMap)
{
  auto registry = sys::new_map_document_registry();
  auto& map = ctx_get<comp::MapInfo>(registry);

  sys::resize_map(registry, 12u, 3u);
  ASSERT_EQ(12u, map.row_count);
  ASSERT_EQ(3u, map.column_count);

  sys::resize_map(registry, 0u, 0u);
  ASSERT_EQ(1u, map.row_count);
  ASSERT_EQ(1u, map.column_count);
}

TEST(MapSystem, FixTilesInMap)
{
  auto registry = sys::new_map_document_registry();
  const auto& info = ctx_get<comp::MapInfo>(registry);

  const auto layerEntity = sys::new_tile_layer(registry);
  auto& layer = checked_get<comp::TileLayer>(registry, layerEntity);

  // TODO test with real tileset
  invoke_mn(info.row_count, info.column_count, [&](usize r, usize c) {
    sys::set_tile(layer, TilePos::from(r, c), next_random(1, 10));
  });

  sys::fix_tiles_in_map(registry);

  invoke_mn(info.row_count, info.column_count, [&](usize r, usize c) {
    ASSERT_EQ(empty_tile, sys::get_tile(layer, TilePos::from(r, c)));
  });
}

TEST(MapSystem, IsPositionInMap)
{
  const auto registry = sys::new_map_document_registry();
  const auto& map = ctx_get<comp::MapInfo>(registry);

  invoke_mn(map.row_count, map.column_count, [&](usize r, usize c) {
    ASSERT_TRUE(sys::is_position_in_map(registry, TilePos::from(r, c)));
  });

  // clang-format off
  ASSERT_FALSE(sys::is_position_in_map(registry, TilePos::from(map.row_count, map.column_count)));
  ASSERT_FALSE(sys::is_position_in_map(registry, TilePos::from(map.row_count - 1u, map.column_count)));
  ASSERT_FALSE(sys::is_position_in_map(registry, TilePos::from(map.row_count, map.column_count - 1u)));
  // clang-format on
}