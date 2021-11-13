#pragma once

#include <tactile_def.hpp>

#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseCSV(CStr csv, TileLayer& layer) -> ParseError;

}  // namespace Tactile::IO
