#include "map_system.hpp"

#include <type_traits>  // enable_if_t, is_unsigned_v

#include <tactile_stdlib.hpp>

#include "assert.hpp"
#include "core/algorithms/invoke_n.hpp"
#include "core/components/layer.hpp"
#include "core/map.hpp"
#include "core/systems/layers/tile_layer_system.hpp"

namespace tactile::sys {
namespace {

template <typename T>
using Unsigned = std::enable_if_t<std::is_unsigned_v<T>, int>;

template <typename T, Unsigned<T> = 0>
[[nodiscard]] constexpr auto GetDiff(const T a, const T b) noexcept -> T
{
  if (a < b) {
    return b - a;
  }
  else {
    return a - b;
  }
}

}  // namespace

void AddRow(entt::registry& registry)
{
  auto& map = registry.ctx<Map>();
  ++map.row_count;

  for (auto&& [entity, layer] : registry.view<TileLayer>().each()) {
    layer.matrix.push_back(MakeTileRow(map.column_count));
  }
}

void AddColumn(entt::registry& registry)
{
  auto& map = registry.ctx<Map>();
  ++map.column_count;

  for (auto&& [entity, layer] : registry.view<TileLayer>().each()) {
    for (auto& row : layer.matrix) {
      row.push_back(empty_tile);
    }
  }
}

void RemoveRow(entt::registry& registry)
{
  auto& map = registry.ctx<Map>();
  if (map.row_count > 1) {
    --map.row_count;
    for (auto&& [entity, layer] : registry.view<TileLayer>().each()) {
      layer.matrix.pop_back();
    }
  }
}

void RemoveColumn(entt::registry& registry)
{
  auto& map = registry.ctx<Map>();
  if (map.column_count > 1) {
    --map.column_count;

    for (auto&& [entity, layer] : registry.view<TileLayer>().each()) {
      for (auto& row : layer.matrix) {
        TACTILE_ASSERT(row.size() > 1);
        row.pop_back();
      }
    }
  }
}

void ResizeMap(entt::registry& registry, const usize nRows, const usize nCols)
{
  auto& map = registry.ctx<Map>();

  if (const auto diff = GetDiff(map.row_count, nRows); map.row_count < nRows) {
    invoke_n(diff, [&] { AddRow(registry); });
  }
  else {
    invoke_n(diff, [&] { RemoveRow(registry); });
  }

  if (const auto diff = GetDiff(map.column_count, nCols); map.column_count < nCols) {
    invoke_n(diff, [&] { AddColumn(registry); });
  }
  else {
    invoke_n(diff, [&] { RemoveColumn(registry); });
  }
}

auto IsPositionInMap(const entt::registry& registry, const MapPosition& position) -> bool
{
  const auto& map = registry.ctx<Map>();

  const auto row = position.GetRow();
  const auto column = position.GetColumn();

  return (row >= 0) && (column >= 0) && (static_cast<usize>(row) < map.row_count) &&
         (static_cast<usize>(column) < map.column_count);
}

}  // namespace tactile::sys
