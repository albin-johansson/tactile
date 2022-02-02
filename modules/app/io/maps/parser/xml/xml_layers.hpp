#pragma once

#include <pugixml.hpp>

#include "io/maps/parser/parse_data.hpp"

namespace tactile::parsing {

[[nodiscard]] auto parse_object(pugi::xml_node objectNode, ir::object_data& objectData)
    -> parse_error;

[[nodiscard]] auto parse_layers(pugi::xml_node mapNode, ir::map_data& mapData)
    -> parse_error;

}  // namespace tactile::parsing