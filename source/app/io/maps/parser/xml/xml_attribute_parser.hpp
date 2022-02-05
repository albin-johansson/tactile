#pragma once

#include <pugixml.hpp>

#include "io/maps/parser/parse_data.hpp"

namespace tactile::parsing {

[[nodiscard]] auto parse_properties(pugi::xml_node node,
                                    ir::attribute_context_data& context) -> parse_error;

}  // namespace tactile::parsing