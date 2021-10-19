#pragma once

#include <vector>  // vector

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(pugi::xml_node root, std::vector<LayerData>& layers)
    -> ParseError;

}  // namespace Tactile::IO
