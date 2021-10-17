#pragma once

#include "common/expected.hpp"
#include "common/ints.hpp"
#include "common/tile_matrix.hpp"
#include "io/json.hpp"
#include "parsers/parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileData(const JSON& json, int32 nRows, int32 nCols)
    -> Expected<TileMatrix, ParseError>;

}  // namespace Tactile::IO
