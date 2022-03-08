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

#include "point_tool_system.hpp"

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "editor/events/tool_events.hpp"

namespace tactile::sys {

void on_point_tool_pressed(entt::registry& registry,
                           entt::dispatcher& dispatcher,
                           const mouse_info& mouse)
{
  if (is_object_layer_active(registry) && mouse.button == cen::mouse_button::left) {
    const auto [xRatio, yRatio] = GetViewportScalingRatio(registry);

    const auto x = mouse.x / xRatio;
    const auto y = mouse.y / yRatio;

    dispatcher.enqueue<AddPointEvent>(x, y);
  }
}

}  // namespace tactile::sys