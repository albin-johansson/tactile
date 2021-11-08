#pragma once

#include <tactile_def.hpp>

#include <entt.hpp>  // registry, entity

#include "common/tile_cache.hpp"
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
