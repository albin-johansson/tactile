#pragma once

#include <entt/entt.hpp>  // registry, dispatcher

#include "core/mouse_info.hpp"

namespace tactile::sys {

void StampToolOnPressed(entt::registry& registry, const mouse_info& mouse);

void StampToolOnDragged(entt::registry& registry, const mouse_info& mouse);

void StampToolOnReleased(entt::registry& registry,
                         entt::dispatcher& dispatcher,
                         const mouse_info& mouse);

}  // namespace tactile::sys
