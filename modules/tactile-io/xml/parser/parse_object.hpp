#pragma once

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseObject(pugi::xml_node node, Object& object) -> ParseError;

}  // namespace tactile::IO
