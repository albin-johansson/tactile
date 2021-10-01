#pragma once

#include "common/ints.hpp"
#include "common/json.hpp"
#include "common/tile_matrix.hpp"
#include "io/parsing/parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileData(const JSON& json, int32 nCols, TileMatrix& matrix)
    -> ParseError;

}  // namespace Tactile::IO
