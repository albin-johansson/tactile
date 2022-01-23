#pragma once

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseFancyTiles(pugi::xml_node node, Tileset& tileset) -> ParseError;

}  // namespace tactile::IO
