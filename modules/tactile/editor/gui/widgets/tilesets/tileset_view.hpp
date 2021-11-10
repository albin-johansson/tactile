#pragma once

#include <entt/entt.hpp>  // registry, entity, dispatcher

namespace Tactile {

void UpdateTilesetView(const entt::registry& registry,
                       entt::entity entity,
                       entt::dispatcher& dispatcher);

}  // namespace Tactile
