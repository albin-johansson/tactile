#pragma once

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseFancyTiles(const JSON& json, Tileset& tileset) -> ParseError;

}  // namespace tactile::IO
