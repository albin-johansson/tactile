#pragma once

#include <entt.hpp>  // registry, dispatcher

#include "core/ctx/mouse_info.hpp"

namespace Tactile::Sys {

void BucketToolOnPressed(entt::registry& registry,
                         entt::dispatcher& dispatcher,
                         const MouseInfo& mouse);

}  // namespace Tactile::Sys
