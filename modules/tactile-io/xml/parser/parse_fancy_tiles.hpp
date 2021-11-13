#pragma once

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseFancyTiles(pugi::xml_node node, Tileset& tileset) -> ParseError;

}  // namespace Tactile::IO
