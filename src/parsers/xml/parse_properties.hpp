#pragma once

#include <vector>  // vector

#include <pugixml.hpp>  // xml_node

#include <tactile/io/maps/parse_error.hpp>
#include <tactile/io/maps/parse_ir.hpp>

namespace Tactile::IO {

[[nodiscard]] auto ParseProperties(pugi::xml_node node,
                                   std::vector<PropertyData>& properties) -> ParseError;

}  // namespace Tactile::IO
