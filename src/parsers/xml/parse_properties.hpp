#pragma once

#include <pugixml.hpp>  // xml_node
#include <vector>       // vector

#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseProperties(pugi::xml_node node,
                                   std::vector<PropertyData>& properties) -> ParseError;

}  // namespace Tactile::IO
