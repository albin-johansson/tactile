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

#include <entt/fwd.hpp>

#include "core/mouse_info.hpp"
#include "core/tool_type.hpp"

namespace tactile::sys {

void select_tool(entt::registry& registry, entt::dispatcher& dispatcher, ToolType tool);

void on_tool_entered(entt::registry& registry, entt::dispatcher& dispatcher);

void on_tool_exited(entt::registry& registry, entt::dispatcher& dispatcher);

void on_tool_pressed(entt::registry& registry,
                     entt::dispatcher& dispatcher,
                     const MouseInfo& mouse);

void on_tool_dragged(entt::registry& registry,
                     entt::dispatcher& dispatcher,
                     const MouseInfo& mouse);

void on_tool_released(entt::registry& registry,
                      entt::dispatcher& dispatcher,
                      const MouseInfo& mouse);

[[nodiscard]] auto is_tool_enabled(const entt::registry& registry, ToolType tool) -> bool;

}  // namespace tactile::sys
