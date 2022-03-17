#include "core/systems/map_system.hpp"

#include <gtest/gtest.h>

#include "core/algorithms/invoke_n.hpp"
#include "core/map.hpp"
#include "core/systems/registry_system.hpp"

using namespace tactile;

TEST(MapSystem, AddRow)
{
  auto registry = sys::make_document_registry();
  auto& map = registry.ctx<MapInfo>();
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
  auto registry = sys::make_document_registry();
  auto& map = registry.ctx<MapInfo>();
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
  auto registry = sys::make_document_registry();
  auto& map = registry.ctx<MapInfo>();
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
  auto registry = sys::make_document_registry();
  auto& map = registry.ctx<MapInfo>();
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
  auto registry = sys::make_document_registry();
  auto& map = registry.ctx<MapInfo>();

  sys::resize_map(registry, 12u, 3u);
  ASSERT_EQ(12u, map.row_count);
  ASSERT_EQ(3u, map.column_count);

  sys::resize_map(registry, 0u, 0u);
  ASSERT_EQ(1u, map.row_count);
  ASSERT_EQ(1u, map.column_count);
}

TEST(MapSystem, IsPositionInMap)
{
  const auto registry = sys::make_document_registry();
  const auto& map = registry.ctx<MapInfo>();

  for (usize r = 0; r < map.row_count; ++r) {
    for (usize c = 0; c < map.column_count; ++c) {
      ASSERT_TRUE(sys::is_position_in_map(registry, TilePos::from(r, c)));
    }
  }

  // clang-format off
  ASSERT_FALSE(sys::is_position_in_map(registry, TilePos::from(map.row_count, map.column_count)));
  ASSERT_FALSE(sys::is_position_in_map(registry, TilePos::from(map.row_count - 1u, map.column_count)));
  ASSERT_FALSE(sys::is_position_in_map(registry, TilePos::from(map.row_count, map.column_count - 1u)));
  // clang-format on
}