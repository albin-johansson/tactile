#pragma once

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileData(pugi::xml_node node, TileLayer& layer) -> ParseError;

}  // namespace Tactile::IO
