#pragma once

#include <entt/entt.hpp>  // registry, dispatcher

#include "core/mouse_info.hpp"

namespace tactile::sys {

void BucketToolOnPressed(entt::registry& registry,
                         entt::dispatcher& dispatcher,
                         const mouse_info& mouse);

}  // namespace tactile::sys
