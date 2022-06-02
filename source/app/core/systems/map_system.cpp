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

#include <cmath>
#include <concepts>  // unsigned_integral

#include <entt/entity/registry.hpp>

#include "core/algorithms/invoke.hpp"
#include "core/common/ecs.hpp"
#include "core/common/math.hpp"
#include "core/components/attributes.hpp"
#include "core/components/layers.hpp"
#include "core/components/map_info.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/tilesets/tileset_system.hpp"
#include "core/utils/tiles.hpp"
#include "misc/assert.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _fix_tiles_in_layer(comp::TileLayer& layer,
                                       const comp::TilesetContext& tilesets,
                                       const usize rows,
                                       const usize cols) -> HashMap<TilePos, TileID>
{
  HashMap<TilePos, TileID> previous;

  invoke_mn(rows, cols, [&](const usize row, const usize col) {
    const auto tileId = layer.matrix[row][col];
    if (!tilesets.tile_to_tileset.contains(tileId)) {
      previous[TilePos::from(row, col)] = layer.matrix[row][col];
      layer.matrix[row][col] = empty_tile;
    }
  });

  return previous;
}

}  // namespace

void add_row_to_map(entt::registry& registry)
{
  auto& map = ctx_get<comp::MapInfo>(registry);
  ++map.row_count;

  for (auto&& [entity, layer] : registry.view<comp::TileLayer>().each()) {
    layer.matrix.push_back(make_tile_row(map.column_count));
  }
}

void add_column_to_map(entt::registry& registry)
{
  auto& map = ctx_get<comp::MapInfo>(registry);
  ++map.column_count;

  for (auto&& [entity, layer] : registry.view<comp::TileLayer>().each()) {
    for (auto& row : layer.matrix) {
      row.push_back(empty_tile);
    }
  }
}

void remove_row_from_map(entt::registry& registry)
{
  auto& map = ctx_get<comp::MapInfo>(registry);
  if (map.row_count > 1) {
    --map.row_count;
    for (auto&& [entity, layer] : registry.view<comp::TileLayer>().each()) {
      layer.matrix.pop_back();
    }
  }
}

void remove_column_from_map(entt::registry& registry)
{
  auto& map = ctx_get<comp::MapInfo>(registry);
  if (map.column_count > 1) {
    --map.column_count;

    for (auto&& [entity, layer] : registry.view<comp::TileLayer>().each()) {
      for (auto& row : layer.matrix) {
        TACTILE_ASSERT(row.size() > 1);
        row.pop_back();
      }
    }
  }
}

void resize_map(entt::registry& registry, const usize nRows, const usize nCols)
{
  auto& map = ctx_get<comp::MapInfo>(registry);

  if (const auto diff = udiff(map.row_count, nRows); map.row_count < nRows) {
    invoke_n(diff, [&] { add_row_to_map(registry); });
  }
  else {
    invoke_n(diff, [&] { remove_row_from_map(registry); });
  }

  if (const auto diff = udiff(map.column_count, nCols); map.column_count < nCols) {
    invoke_n(diff, [&] { add_column_to_map(registry); });
  }
  else {
    invoke_n(diff, [&] { remove_column_from_map(registry); });
  }
}

auto fix_tiles_in_map(entt::registry& registry) -> FixTilesInMapResult
{
  FixTilesInMapResult result;

  const auto& info = ctx_get<comp::MapInfo>(registry);
  const auto& tilesets = ctx_get<comp::TilesetContext>(registry);

  for (auto&& [entity, layer, context] :
       registry.view<comp::TileLayer, comp::AttributeContext>().each()) {
    const auto& id = context.id;
    result[id] = _fix_tiles_in_layer(layer, tilesets, info.row_count, info.column_count);
  }

  return result;
}

auto is_position_in_map(const entt::registry& registry, const TilePos& position) -> bool
{
  const auto& map = ctx_get<comp::MapInfo>(registry);

  const auto row = position.row();
  const auto column = position.col();

  return (row >= 0) && (column >= 0) && (static_cast<usize>(row) < map.row_count) &&
         (static_cast<usize>(column) < map.column_count);
}

}  // namespace tactile::sys
