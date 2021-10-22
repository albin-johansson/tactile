#pragma once

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileData(const JSON& json, TileLayer& layer) -> ParseError;

}  // namespace Tactile::IO
