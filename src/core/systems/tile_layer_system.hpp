#pragma once

#include <entt.hpp>  // registry, entity

#include "aliases/col.hpp"
#include "aliases/row.hpp"
#include "aliases/tile_id.hpp"
#include "aliases/tile_matrix.hpp"
#include "core/map_position.hpp"

namespace Tactile::Sys {

/// \name Tile layer system
/// \{

void SetTileInLayer(entt::registry& registry,
                    entt::entity entity,
                    const MapPosition& position,
                    tile_id tile);

[[nodiscard]] auto MakeTileRow(col_t nCols, tile_id value) -> TileRow;

[[nodiscard]] auto MakeTileMatrix(row_t nRows, col_t nCols) -> TileMatrix;

[[nodiscard]] auto GetTileFromLayer(const entt::registry& registry,
                                    entt::entity entity,
                                    const MapPosition& position) -> tile_id;

/// \} End of tile layer system

}  // namespace Tactile::Sys
