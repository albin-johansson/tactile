#pragma once

#include <pugixml.hpp>  // xml_node

#include "ir.hpp"
#include "parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileData(pugi::xml_node node, TileLayer& layer) -> ParseError;

}  // namespace Tactile::IO
