#pragma once

#include <tactile-base/tactile_std.hpp>

#include "parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseCSV(CStr csv, int32 nCols, TileMatrix& matrix) -> ParseError;

}  // namespace Tactile::IO
