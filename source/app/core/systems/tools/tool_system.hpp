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

#pragma once

#include <entt/entt.hpp>

#include "core/mouse_info.hpp"
#include "core/tool_type.hpp"

namespace tactile::sys {

void select_tool(entt::registry& registry, entt::dispatcher& dispatcher, tool_type tool);

void on_tool_pressed(entt::registry& registry,
                     entt::dispatcher& dispatcher,
                     const mouse_info& mouse);

void on_tool_dragged(entt::registry& registry,
                     entt::dispatcher& dispatcher,
                     const mouse_info& mouse);

void on_tool_released(entt::registry& registry,
                      entt::dispatcher& dispatcher,
                      const mouse_info& mouse);

[[nodiscard]] auto is_tool_enabled(const entt::registry& registry, tool_type tool)
    -> bool;

}  // namespace tactile::sys
