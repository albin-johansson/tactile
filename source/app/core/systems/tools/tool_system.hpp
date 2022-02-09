#pragma once

#include <entt/entt.hpp>  // registry, dispatcher

#include "core/mouse_info.hpp"
#include "core/tool_type.hpp"

namespace tactile::sys {

void SelectTool(entt::registry& registry, tool_type tool);

void ToolOnPressed(entt::registry& registry,
                   entt::dispatcher& dispatcher,
                   const mouse_info& mouse);

void ToolOnDragged(entt::registry& registry,
                   entt::dispatcher& dispatcher,
                   const mouse_info& mouse);

void ToolOnReleased(entt::registry& registry,
                    entt::dispatcher& dispatcher,
                    const mouse_info& mouse);

[[nodiscard]] auto is_tool_enabled(const entt::registry& registry, tool_type tool)
    -> bool;

}  // namespace tactile::sys
