#pragma once

#include <pugixml.hpp>  // xml_node

#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObject(pugi::xml_node node, ObjectData& object) -> ParseError;

}  // namespace Tactile::IO
