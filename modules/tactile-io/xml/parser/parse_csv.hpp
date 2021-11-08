#pragma once

#include <tactile_def.hpp>

#include "ir.hpp"
#include "parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseCSV(CStr csv, TileLayer& layer) -> ParseError;

}  // namespace Tactile::IO
