#pragma once

#include "aliases/cstr.hpp"
#include "aliases/ints.hpp"
#include "aliases/tile_matrix.hpp"
#include "io/parsing/parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseCSV(CStr csv, int32 nCols, TileMatrix& matrix) -> ParseError;

}  // namespace Tactile::IO
