#include "core/systems/map_system.hpp"

#include <gtest/gtest.h>

#include "core/algorithms/invoke_n.hpp"
#include "core/map.hpp"
#include "core/systems/registry_system.hpp"

using namespace Tactile;

TEST(MapSystem, AddRow)
{
  auto registry = Sys::MakeRegistry();
  auto& map = registry.ctx<Map>();
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  Sys::AddRow(registry);
  ASSERT_EQ(6u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  Sys::AddRow(registry);
  Sys::AddRow(registry);
  ASSERT_EQ(8u, map.row_count);
  ASSERT_EQ(5u, map.column_count);
}

TEST(MapSystem, AddColumn)
{
  auto registry = Sys::MakeRegistry();
  auto& map = registry.ctx<Map>();
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  Sys::AddColumn(registry);
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(6u, map.column_count);

  Sys::AddColumn(registry);
  Sys::AddColumn(registry);
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(8u, map.column_count);
}

TEST(MapSystem, RemoveRow)
{
  auto registry = Sys::MakeRegistry();
  auto& map = registry.ctx<Map>();
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  Sys::RemoveRow(registry);
  ASSERT_EQ(4u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  InvokeN(4, [&] { Sys::RemoveRow(registry); });
  ASSERT_EQ(1u, map.row_count);
  ASSERT_EQ(5u, map.column_count);
}

TEST(MapSystem, RemoveColumn)
{
  auto registry = Sys::MakeRegistry();
  auto& map = registry.ctx<Map>();
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(5u, map.column_count);

  Sys::RemoveColumn(registry);
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(4u, map.column_count);

  InvokeN(4, [&] { Sys::RemoveColumn(registry); });
  ASSERT_EQ(5u, map.row_count);
  ASSERT_EQ(1u, map.column_count);
}

TEST(MapSystem, ResizeMap)
{
  auto registry = Sys::MakeRegistry();
  auto& map = registry.ctx<Map>();

  Sys::ResizeMap(registry, 12u, 3u);
  ASSERT_EQ(12u, map.row_count);
  ASSERT_EQ(3u, map.column_count);

  Sys::ResizeMap(registry, 0u, 0u);
  ASSERT_EQ(1u, map.row_count);
  ASSERT_EQ(1u, map.column_count);
}

TEST(MapSystem, IsPositionInMap)
{
  const auto registry = Sys::MakeRegistry();
  const auto& map = registry.ctx<Map>();

  for (usize r = 0; r < map.row_count; ++r) {
    for (usize c = 0; c < map.column_count; ++c) {
      ASSERT_TRUE(Sys::IsPositionInMap(registry, {r, c}));
    }
  }

  ASSERT_FALSE(Sys::IsPositionInMap(registry, {map.row_count, map.column_count}));
  ASSERT_FALSE(Sys::IsPositionInMap(registry, {map.row_count - 1u, map.column_count}));
  ASSERT_FALSE(Sys::IsPositionInMap(registry, {map.row_count, map.column_count - 1u}));
}