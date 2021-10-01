#pragma once

#include <entt.hpp>  // registry, entity
#include <vector>    // vector

#include "common/tile_id.hpp"
#include "core/map_position.hpp"

namespace Tactile {

void FloodFill(entt::registry& registry,
               entt::entity entity,
               const MapPosition& origin,
               TileID replacement,
               std::vector<MapPosition>& affected);

}  // namespace Tactile
