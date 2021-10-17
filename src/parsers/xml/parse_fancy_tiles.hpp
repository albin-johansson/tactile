#pragma once

#include <pugixml.hpp>  // xml_node

#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseFancyTiles(pugi::xml_node node, TilesetData& data) -> ParseError;

}  // namespace Tactile::IO
