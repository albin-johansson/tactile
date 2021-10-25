#pragma once

#include <pugixml.hpp>  // xml_node

#include "ir.hpp"
#include "parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseFancyTiles(pugi::xml_node node, Tileset& tileset) -> ParseError;

}  // namespace Tactile::IO
