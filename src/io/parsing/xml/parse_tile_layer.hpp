#pragma once

#include <pugixml.hpp>  // xml_node

#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(pugi::xml_node node, LayerData& layer)
    -> ParseError;

}  // namespace Tactile::IO
