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

#include "animation_system.hpp"

#include <centurion/system.hpp>
#include <entt/entity/registry.hpp>

#include "core/components/animation.hpp"

namespace tactile::sys {

void update_animations(entt::registry& registry)
{
  for (auto&& [entity, animation] : registry.view<comp::Animation>().each()) {
    const auto now = cen::ticks64();

    const auto frameEntity = animation.frames.at(animation.index);
    const auto& currentFrame = registry.get<comp::AnimationFrame>(frameEntity);

    if (now - animation.last_update_time >= currentFrame.duration) {
      animation.last_update_time = now;
      animation.index = (animation.index + 1) % animation.frames.size();
    }
  }
}

}  // namespace tactile::sys
