#pragma once

#include <entt/entt.hpp>  // registry, dispatcher

#include "core/mouse_info.hpp"

namespace tactile::Sys {

void BucketToolOnPressed(entt::registry& registry,
                         entt::dispatcher& dispatcher,
                         const MouseInfo& mouse);

}  // namespace tactile::Sys
