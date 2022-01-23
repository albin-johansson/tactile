#include "core/systems/map_system.hpp"

#include <gtest/gtest.h>

#include "core/algorithms/invoke_n.hpp"
#include "core/map.hpp"
#include "core/systems/registry_system.hpp"

using namespace tactile;

TEST(MapSystem, AddRow)
{
  auto registry = sys::MakeRegistry();
  auto& map = registry.ctx<Map>();
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  sys::AddRow(registry);
  ASSERT_EQ(6u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  sys::AddRow(registry);
  sys::AddRow(registry);
  ASSERT_EQ(8u, map.row_count);
  ASSERT_EQ(5u, map.column_count);
}

TEST(MapSystem, AddColumn)
{
  auto registry = sys::MakeRegistry();
  auto& map = registry.ctx<Map>();
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  sys::AddColumn(registry);
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(6u, map.column_count);

  sys::AddColumn(registry);
  sys::AddColumn(registry);
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(8u, map.column_count);
}

TEST(MapSystem, RemoveRow)
{
  auto registry = sys::MakeRegistry();
  auto& map = registry.ctx<Map>();
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  sys::RemoveRow(registry);
  ASSERT_EQ(4u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  InvokeN(4, [&] { sys::RemoveRow(registry); });
  ASSERT_EQ(1u, map.row_count);
  ASSERT_EQ(5u, map.column_count);
}

TEST(MapSystem, RemoveColumn)
{
  auto registry = sys::MakeRegistry();
  auto& map = registry.ctx<Map>();
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  sys::RemoveColumn(registry);
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(4u, map.column_count);

  InvokeN(4, [&] { sys::RemoveColumn(registry); });
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(1u, map.column_count);
}

TEST(MapSystem, ResizeMap)
{
  auto registry = sys::MakeRegistry();
  auto& map = registry.ctx<Map>();

  sys::ResizeMap(registry, 12u, 3u);
  ASSERT_EQ(12u, map.row_count);
  ASSERT_EQ(3u, map.column_count);

  sys::ResizeMap(registry, 0u, 0u);
  ASSERT_EQ(1u, map.row_count);
  ASSERT_EQ(1u, map.column_count);
}

TEST(MapSystem, IsPositionInMap)
{
  const auto registry = sys::MakeRegistry();
  const auto& map = registry.ctx<Map>();

  for (usize r = 0; r < map.row_count; ++r) {
    for (usize c = 0; c < map.column_count; ++c) {
      ASSERT_TRUE(sys::IsPositionInMap(registry, MapPosition::From(r, c)));
    }
  }

  // clang-format off
  ASSERT_FALSE(sys::IsPositionInMap(registry, MapPosition::From(map.row_count, map.column_count)));
  ASSERT_FALSE(sys::IsPositionInMap(registry, MapPosition::From(map.row_count - 1u, map.column_count)));
  ASSERT_FALSE(sys::IsPositionInMap(registry, MapPosition::From(map.row_count, map.column_count - 1u)));
  // clang-format on
}