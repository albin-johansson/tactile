#pragma once

#include <entt/entt.hpp>  // registry, dispatcher

#include "core/mouse_info.hpp"

namespace tactile::sys {

void ObjectSelectionToolOnPressed(entt::registry& registry,
                                  entt::dispatcher& dispatcher,
                                  const mouse_info& mouse);

void ObjectSelectionToolOnDragged(entt::registry& registry, const mouse_info& mouse);

void ObjectSelectionToolOnReleased(entt::registry& registry,
                                   entt::dispatcher& dispatcher,
                                   const mouse_info& mouse);

}  // namespace tactile::sys
