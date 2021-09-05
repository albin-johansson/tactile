#pragma once

#include <entt.hpp>  // registry, entity
#include <vector>    // vector

#include "aliases/tile_id.hpp"
#include "core/map_position.hpp"

namespace Tactile {

void FloodFill(entt::registry& registry,
               entt::entity entity,
               const MapPosition& origin,
               tile_id replacement,
               std::vector<MapPosition>& affected);

}  // namespace Tactile
