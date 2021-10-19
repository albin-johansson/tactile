#pragma once

#include <entt.hpp>  // registry, dispatcher

#include "core/mouse_info.hpp"

namespace Tactile::Sys {

void StampToolOnPressed(entt::registry& registry, const MouseInfo& mouse);

void StampToolOnDragged(entt::registry& registry, const MouseInfo& mouse);

void StampToolOnReleased(entt::registry& registry,
                         entt::dispatcher& dispatcher,
                         const MouseInfo& mouse);

}  // namespace Tactile::Sys
