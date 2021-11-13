#pragma once

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileData(const JSON& json, TileLayer& layer) -> ParseError;

}  // namespace Tactile::IO
