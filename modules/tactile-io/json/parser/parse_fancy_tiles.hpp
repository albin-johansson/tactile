#pragma once

#include "../json_common.hpp"
#include "ir.hpp"
#include "parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseFancyTiles(const JSON& json, Tileset& tileset) -> ParseError;

}  // namespace Tactile::IO
