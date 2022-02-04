#pragma once

#include <entt/entt.hpp>  // registry, dispatcher

#include "core/mouse_info.hpp"

namespace tactile::sys {

void EraserToolOnPressed(entt::registry& registry, const mouse_info& mouse);

void EraserToolOnDragged(entt::registry& registry, const mouse_info& mouse);

void EraserToolOnReleased(entt::registry& registry,
                          entt::dispatcher& dispatcher,
                          const mouse_info& mouse);

}  // namespace tactile::sys