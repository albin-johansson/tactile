#pragma once

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(pugi::xml_node node, LayerData& layer) -> ParseError;

}  // namespace Tactile::IO
