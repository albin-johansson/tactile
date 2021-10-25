#pragma once

#include <entt.hpp>  // registry, entity, dispatcher

namespace Tactile {

void UpdateTilesetView(const entt::registry& registry,
                       entt::entity entity,
                       entt::dispatcher& dispatcher);

}  // namespace Tactile
