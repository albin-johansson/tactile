/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "map_system.hpp"

#include "core/algorithms/invoke_n.hpp"
#include "core/components/layer.hpp"
#include "core/map.hpp"
#include "core/utils/sfinae.hpp"
#include "core/utils/tiles.hpp"
#include "layers/tile_layer_system.hpp"
#include "misc/assert.hpp"

namespace tactile::sys {
namespace {

template <typename T, is_unsigned<T> = 0>
[[nodiscard]] constexpr auto get_diff(const T a, const T b) noexcept -> T
{
  if (a < b) {
    return b - a;
  }
  else {
    return a - b;
  }
}

}  // namespace

void add_row_to_map(entt::registry& registry)
{
  auto& map = registry.ctx<MapInfo>();
  ++map.row_count;

  for (auto&& [entity, layer] : registry.view<comp::tile_layer>().each()) {
    layer.matrix.push_back(make_tile_row(map.column_count));
  }
}

void add_column_to_map(entt::registry& registry)
{
  auto& map = registry.ctx<MapInfo>();
  ++map.column_count;

  for (auto&& [entity, layer] : registry.view<comp::tile_layer>().each()) {
    for (auto& row : layer.matrix) {
      row.push_back(empty_tile);
    }
  }
}

void remove_row_from_map(entt::registry& registry)
{
  auto& map = registry.ctx<MapInfo>();
  if (map.row_count > 1) {
    --map.row_count;
    for (auto&& [entity, layer] : registry.view<comp::tile_layer>().each()) {
      layer.matrix.pop_back();
    }
  }
}

void remove_column_from_map(entt::registry& registry)
{
  auto& map = registry.ctx<MapInfo>();
  if (map.column_count > 1) {
    --map.column_count;

    for (auto&& [entity, layer] : registry.view<comp::tile_layer>().each()) {
      for (auto& row : layer.matrix) {
        TACTILE_ASSERT(row.size() > 1);
        row.pop_back();
      }
    }
  }
}

void resize_map(entt::registry& registry, usize nRows, usize nCols)
{
  auto& map = registry.ctx<MapInfo>();

  if (const auto diff = get_diff(map.row_count, nRows); map.row_count < nRows) {
    invoke_n(diff, [&] { add_row_to_map(registry); });
  }
  else {
    invoke_n(diff, [&] { remove_row_from_map(registry); });
  }

  if (const auto diff = get_diff(map.column_count, nCols); map.column_count < nCols) {
    invoke_n(diff, [&] { add_column_to_map(registry); });
  }
  else {
    invoke_n(diff, [&] { remove_column_from_map(registry); });
  }
}

auto is_position_in_map(const entt::registry& registry, const tile_position& position)
    -> bool
{
  const auto& map = registry.ctx<MapInfo>();

  const auto row = position.row();
  const auto column = position.col();

  return (row >= 0) && (column >= 0) && (static_cast<usize>(row) < map.row_count) &&
         (static_cast<usize>(column) < map.column_count);
}

}  // namespace tactile::sys
