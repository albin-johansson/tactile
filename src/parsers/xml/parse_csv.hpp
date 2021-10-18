#pragma once

#include <tactile-io/parse_error.hpp>

#include <tactile-base/tactile_std.hpp>

namespace Tactile::IO {

[[nodiscard]] auto ParseCSV(CStr csv, int32 nCols, TileMatrix& matrix) -> ParseError;

}  // namespace Tactile::IO
