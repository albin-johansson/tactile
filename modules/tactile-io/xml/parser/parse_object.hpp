#pragma once

#include <pugixml.hpp>  // xml_node

#include "ir.hpp"
#include "parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObject(pugi::xml_node node, Object& object) -> ParseError;

}  // namespace Tactile::IO
