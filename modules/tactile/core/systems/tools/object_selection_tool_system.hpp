#pragma once

#include <entt/entt.hpp>  // registry, dispatcher

#include "core/mouse_info.hpp"

namespace tactile::Sys {

void ObjectSelectionToolOnPressed(entt::registry& registry,
                                  entt::dispatcher& dispatcher,
                                  const MouseInfo& mouse);

void ObjectSelectionToolOnDragged(entt::registry& registry, const MouseInfo& mouse);

void ObjectSelectionToolOnReleased(entt::registry& registry,
                                   entt::dispatcher& dispatcher,
                                   const MouseInfo& mouse);

}  // namespace tactile::Sys
