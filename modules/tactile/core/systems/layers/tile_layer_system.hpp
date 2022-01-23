#pragma once

#include <entt/entt.hpp>  // registry, entity
#include <tactile_def.hpp>

#include "core/map_position.hpp"
#include "core/tile_cache.hpp"

namespace tactile::sys {

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

}  // namespace tactile::sys
