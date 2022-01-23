#pragma once

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseLayers(pugi::xml_node root, Map& map) -> ParseError;

}  // namespace tactile::IO
