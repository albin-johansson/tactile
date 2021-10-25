#pragma once

#include <vector>  // vector

#include <entt.hpp>  // registry, entity

#include <tactile-base/map_position.hpp>
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

void FloodFill(entt::registry& registry,
               entt::entity entity,
               const MapPosition& origin,
               TileID replacement,
               std::vector<MapPosition>& affected);

}  // namespace Tactile
