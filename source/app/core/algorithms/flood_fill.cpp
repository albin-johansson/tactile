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

#include "flood_fill.hpp"

#include <queue>  // queue

#include <entt/entity/registry.hpp>

#include "core/common/ecs.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/map_system.hpp"
#include "core/tile_pos.hpp"

namespace tactile {

void flood(entt::registry& registry,
           const entt::entity entity,
           const TilePos& origin,
           const TileID replacement,
           std::vector<TilePos>& affected)
{
  auto& layer = checked_get<comp::TileLayer>(registry, entity);
  const auto target = sys::get_tile(layer, origin);

  if (!sys::is_position_in_map(registry, origin) || (target == replacement)) {
    return;
  }

  std::queue<TilePos> positions;
  positions.push(origin);

  sys::set_tile(layer, origin, replacement);
  affected.push_back(origin);

  auto update = [&](const TilePos& position) {
    if (sys::is_position_in_map(registry, position)) {
      const auto tile = sys::get_tile(layer, position);
      if (tile == target) {
        sys::set_tile(layer, position, replacement);
        affected.push_back(position);
        positions.push(position);
      }
    }
  };

  while (!positions.empty()) {
    const auto position = positions.front();
    positions.pop();

    update(position.west());
    update(position.east());
    update(position.south());
    update(position.north());
  }
}

}  // namespace tactile
