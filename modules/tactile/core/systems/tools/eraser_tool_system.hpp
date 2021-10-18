#pragma once

#include <entt.hpp>  // registry, dispatcher

#include "tactile/core/mouse_info.hpp"

namespace Tactile::Sys {

void EraserToolOnPressed(entt::registry& registry, const MouseInfo& mouse);

void EraserToolOnDragged(entt::registry& registry, const MouseInfo& mouse);

void EraserToolOnReleased(entt::registry& registry,
                          entt::dispatcher& dispatcher,
                          const MouseInfo& mouse);

}  // namespace Tactile::Sys