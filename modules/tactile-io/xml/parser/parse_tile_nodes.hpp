#pragma once

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileNodes(const pugi::xml_node& data, TileLayer& layer)
    -> ParseError;

}  // namespace Tactile::IO
