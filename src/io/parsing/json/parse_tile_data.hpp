#pragma once

#include "aliases/col.hpp"
#include "aliases/json.hpp"
#include "aliases/tile_matrix.hpp"
#include "io/parsing/parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileData(const JSON& json, col_t nCols, TileMatrix& matrix)
    -> ParseError;

}  // namespace Tactile::IO
