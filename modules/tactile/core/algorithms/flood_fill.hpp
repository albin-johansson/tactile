#pragma once

#include <vector>  // vector

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "core/map_position.hpp"

namespace tactile {

/**
 * \brief Floods the specified tile layer with a tile at the specified location.
 *
 * \param registry the document registry.
 * \param entity the tile layer entity.
 * \param origin the origin position of the flood fill.
 * \param replacement the tile identifier used to replace the origin identifier.
 * \param[out] affected the positions of the tiles affected by the flood fill.
 */
void flood(entt::registry& registry,
           entt::entity entity,
           const MapPosition& origin,
           TileID replacement,
           std::vector<MapPosition>& affected);

}  // namespace tactile
