#pragma once

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseTileData(pugi::xml_node node, TileLayer& layer) -> ParseError;

}  // namespace tactile::IO
