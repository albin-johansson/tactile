#pragma once

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseFancyTiles(const JSON& json, Tileset& tileset) -> ParseError;

}  // namespace Tactile::IO
