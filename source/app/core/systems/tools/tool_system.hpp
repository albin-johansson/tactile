#pragma once

#include <entt/entt.hpp>

#include "core/mouse_info.hpp"
#include "core/tool_type.hpp"

namespace tactile::sys {

void select_tool(entt::registry& registry, tool_type tool);

void tool_on_pressed(entt::registry& registry,
                     entt::dispatcher& dispatcher,
                     const mouse_info& mouse);

void tool_on_dragged(entt::registry& registry,
                     entt::dispatcher& dispatcher,
                     const mouse_info& mouse);

void tool_on_released(entt::registry& registry,
                      entt::dispatcher& dispatcher,
                      const mouse_info& mouse);

[[nodiscard]] auto is_tool_enabled(const entt::registry& registry, tool_type tool)
    -> bool;

}  // namespace tactile::sys
