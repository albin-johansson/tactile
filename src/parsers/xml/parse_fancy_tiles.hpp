#pragma once

#include <pugixml.hpp>  // xml_node

#include <tactile-io/parse_error.hpp>
#include <tactile-io/parse_ir.hpp>

namespace Tactile::IO {

[[nodiscard]] auto ParseFancyTiles(pugi::xml_node node, TilesetData& data) -> ParseError;

}  // namespace Tactile::IO
