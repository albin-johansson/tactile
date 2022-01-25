#pragma once

#include <entt/entt.hpp>  // registry, entity
#include <tactile_def.hpp>

#include "core/tile_cache.hpp"
#include "core/tile_position.hpp"

namespace tactile::sys {

/// \name Tile layer system
/// \{

void SetTileInLayer(entt::registry& registry,
                    entt::entity entity,
                    const tile_position& position,
                    TileID tile);

void SetTilesInLayer(entt::registry& registry,
                     entt::entity entity,
                     const TileCache& tiles);

[[nodiscard]] auto GetTileFromLayer(const entt::registry& registry,
                                    entt::entity entity,
                                    const tile_position& position) -> TileID;

/// \} End of tile layer system

}  // namespace tactile::sys
