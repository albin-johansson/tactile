#pragma once

#include <pugixml.hpp>  // xml_node

#include "ir.hpp"
#include "parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(pugi::xml_node node, Layer& layer) -> ParseError;

}  // namespace Tactile::IO
