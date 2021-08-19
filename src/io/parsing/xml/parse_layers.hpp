#pragma once

#include <pugixml.hpp>  // xml_node
#include <vector>       // vector

#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(pugi::xml_node root, std::vector<LayerData>& layers)
    -> ParseError;

}  // namespace Tactile::IO
