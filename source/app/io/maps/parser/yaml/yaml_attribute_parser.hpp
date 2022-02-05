#pragma once

#include <yaml-cpp/yaml.h>

#include "io/maps/parser/parse_data.hpp"
#include "tactile.hpp"

namespace tactile::parsing {

[[nodiscard]] auto parse_component_definitions(const YAML::Node& node, ir::map_data& data)
    -> parse_error;

[[nodiscard]] auto parse_properties(const YAML::Node& node,
                                    ir::attribute_context_data& context) -> parse_error;

[[nodiscard]] auto parse_components(const YAML::Node& node,
                                    const ir::map_data& map,
                                    ir::attribute_context_data& context) -> parse_error;

}  // namespace tactile::parsing
