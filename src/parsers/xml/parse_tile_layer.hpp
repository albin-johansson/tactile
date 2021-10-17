#pragma once

#include <pugixml.hpp>  // xml_node

#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(pugi::xml_node node, LayerData& layer) -> ParseError;

}  // namespace Tactile::IO
