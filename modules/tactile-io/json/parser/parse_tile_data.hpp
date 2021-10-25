#pragma once

#include "../json_common.hpp"
#include "ir.hpp"
#include "parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileData(const JSON& json, TileLayer& layer) -> ParseError;

}  // namespace Tactile::IO
