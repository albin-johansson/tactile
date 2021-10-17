#pragma once

#include <entt.hpp>  // registry, entity

#include "common/ints.hpp"
#include "common/tile_cache.hpp"
#include "common/tile_id.hpp"
#include "common/tile_matrix.hpp"
#include "core/map_position.hpp"

namespace Tactile::Sys {

/// \name Tile layer system
/// \{

void SetTileInLayer(entt::registry& registry,
                    entt::entity entity,
                    const MapPosition& position,
                    TileID tile);

void SetTilesInLayer(entt::registry& registry,
                     entt::entity entity,
                     const TileCache& tiles);

[[nodiscard]] auto GetTileFromLayer(const entt::registry& registry,
                                    entt::entity entity,
                                    const MapPosition& position) -> TileID;

/// \} End of tile layer system

}  // namespace Tactile::Sys
