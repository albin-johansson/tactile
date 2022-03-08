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

#include "map_command_cache.hpp"

#include "core/components/layer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"

namespace tactile {

void map_command_cache::clear() noexcept
{
  mCache.clear();
}

void map_command_cache::restore_tiles(entt::registry& registry)
{
  for (const auto& [layerId, tileCache] : mCache) {
    const auto entity = sys::get_tile_layer_entity(registry, layerId);
    auto& layer = registry.get<comp::tile_layer>(entity);

    for (const auto& [position, tileId] : tileCache) {
      sys::set_tile(layer, position, tileId);
    }
  }
}

void map_command_cache::save_tiles(const entt::registry& registry,
                                   const tile_position& begin,
                                   const tile_position& end)
{
  for (auto&& [entity, layer, tileLayer] :
       registry.view<comp::layer, comp::tile_layer>().each()) {
    auto& tileCache = mCache[layer.id];

    const auto endRow = end.row();
    const auto endCol = end.col();
    for (auto row = begin.row(); row < endRow; ++row) {
      for (auto col = begin.col(); col < endCol; ++col) {
        const tile_position position{row, col};
        const auto tile = sys::get_tile(tileLayer, position);
        tileCache.try_emplace(position, tile);
      }
    }
  }
}

void map_command_cache::merge_with(const map_command_cache& other)
{
  for (const auto& [otherLayer, otherTileCache] : other.mCache) {
    auto& tileCache = mCache[otherLayer];
    for (const auto& [otherPos, otherTile] : otherTileCache) {
      tileCache.try_emplace(otherPos, otherTile);
    }
  }
}

}  // namespace tactile
