#pragma once

#include <json.hpp>  // json

#include "aliases/col.hpp"
#include "aliases/tile_matrix.hpp"
#include "io/parsing/parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileData(const nlohmann::json& json,
                                 col_t nCols,
                                 TileMatrix& matrix) -> ParseError;

}  // namespace Tactile::IO
