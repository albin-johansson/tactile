#pragma once

#include <pugixml.hpp>  // xml_node

#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseFancyTiles(pugi::xml_node node, TilesetData& data) -> ParseError;

}  // namespace Tactile::IO
