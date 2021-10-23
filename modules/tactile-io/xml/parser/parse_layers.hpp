#pragma once

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(pugi::xml_node root, Map& map) -> ParseError;

}  // namespace Tactile::IO
