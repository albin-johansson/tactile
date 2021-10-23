#pragma once

#include <tactile-base/tactile_std.hpp>

#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseCSV(CStr csv, TileLayer& layer) -> ParseError;

}  // namespace Tactile::IO
