#pragma once

#include <entt.hpp>  // registry, entity

#include "aliases/ints.hpp"
#include "aliases/tile_cache.hpp"
#include "aliases/tile_id.hpp"
#include "aliases/tile_matrix.hpp"
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

[[nodiscard]] auto MakeTileRow(int32 nCols, TileID value) -> TileRow;

[[nodiscard]] auto MakeTileMatrix(int32 nRows, int32 nCols) -> TileMatrix;

[[nodiscard]] auto GetTileFromLayer(const entt::registry& registry,
                                    entt::entity entity,
                                    const MapPosition& position) -> TileID;

/// \} End of tile layer system

}  // namespace Tactile::Sys
