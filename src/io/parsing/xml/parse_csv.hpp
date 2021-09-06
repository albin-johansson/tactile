#pragma once

#include "aliases/col.hpp"
#include "aliases/cstr.hpp"
#include "aliases/tile_matrix.hpp"
#include "io/parsing/parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseCSV(CStr csv, col_t nCols, TileMatrix& matrix) -> ParseError;

}  // namespace Tactile::IO
