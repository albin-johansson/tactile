#pragma once

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseTileLayer(pugi::xml_node node, Layer& layer) -> ParseError;

}  // namespace tactile::IO
