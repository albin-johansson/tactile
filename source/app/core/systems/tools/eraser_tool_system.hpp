#pragma once

#include <entt/entt.hpp>  // registry, dispatcher

#include "core/mouse_info.hpp"

namespace tactile::sys {

void eraser_tool_on_pressed(entt::registry& registry, const mouse_info& mouse);

void eraser_tool_on_dragged(entt::registry& registry, const mouse_info& mouse);

void eraser_tool_on_released(entt::registry& registry,
                             entt::dispatcher& dispatcher,
                             const mouse_info& mouse);

}  // namespace tactile::sys