#pragma once

#include <pugixml.hpp>  // xml_node

#include <tactile/io/maps/parse_error.hpp>
#include <tactile/io/maps/parse_ir.hpp>

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(pugi::xml_node node, LayerData& layer) -> ParseError;

}  // namespace Tactile::IO
