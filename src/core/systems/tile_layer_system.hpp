#pragma once

#include <entt.hpp>  // registry, entity

#include "aliases/col.hpp"
#include "aliases/maybe.hpp"
#include "aliases/row.hpp"
#include "aliases/tile_id.hpp"
#include "aliases/tile_matrix.hpp"

namespace Tactile::Sys {

/// \name Tile layer system
/// \{

[[nodiscard]] auto MakeTileRow(col_t nCols, tile_id value) -> TileRow;

[[nodiscard]] auto MakeTileMatrix(row_t nRows, col_t nCols) -> TileMatrix;

[[nodiscard]] auto GetTile(const entt::registry& registry,
                           entt::entity entity,
                           row_t row,
                           col_t col) -> Maybe<tile_id>;

/// \} End of tile layer system

}  // namespace Tactile::Sys
