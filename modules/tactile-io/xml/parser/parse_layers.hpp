#pragma once

#include <pugixml.hpp>  // xml_node

#include "ir.hpp"
#include "parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(pugi::xml_node root, Map& map) -> ParseError;

}  // namespace Tactile::IO
