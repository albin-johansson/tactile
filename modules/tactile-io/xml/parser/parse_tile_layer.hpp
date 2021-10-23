#pragma once

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(pugi::xml_node node, Layer& layer) -> ParseError;

}  // namespace Tactile::IO
