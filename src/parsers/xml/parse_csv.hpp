#pragma once

#include <tactile/io/maps/parse_error.hpp>

#include "common/cstr.hpp"
#include "common/ints.hpp"
#include "common/tile_matrix.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseCSV(CStr csv, int32 nCols, TileMatrix& matrix) -> ParseError;

}  // namespace Tactile::IO
