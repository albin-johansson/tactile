/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "tile_animation_system.hpp"

#include "common/debug/assert.hpp"
#include "common/type/chrono.hpp"
#include "model/entity_validation.hpp"
#include "model/tiles/tile_components.hpp"

namespace tactile::sys {

void make_tile_animated(Registry& registry, const Entity tile_entity)
{
  TACTILE_ASSERT(is_tile_entity(registry, tile_entity));
  TACTILE_ASSERT(!registry.has<TileAnimation>(tile_entity));

  const auto& tile = registry.get<Tile>(tile_entity);

  auto& animation = registry.add<TileAnimation>(tile_entity);
  animation.index = 0;
  animation.frames.push_back(TileAnimationFrame {tile.index, ms_t {1'000}});
  animation.last_update = Clock::now();
}

}  // namespace tactile::sys
