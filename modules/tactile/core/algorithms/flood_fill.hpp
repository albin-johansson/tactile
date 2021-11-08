#pragma once

#include <vector>  // vector

#include <tactile_def.hpp>

#include <entt.hpp>  // registry, entity

#include "core/map_position.hpp"

namespace Tactile {

void FloodFill(entt::registry& registry,
               entt::entity entity,
               const MapPosition& origin,
               TileID replacement,
               std::vector<MapPosition>& affected);

}  // namespace Tactile
