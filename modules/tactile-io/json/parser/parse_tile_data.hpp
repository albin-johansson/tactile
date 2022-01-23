#pragma once

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseTileData(const JSON& json, TileLayer& layer) -> ParseError;

}  // namespace tactile::IO
