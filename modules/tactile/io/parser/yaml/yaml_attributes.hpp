#pragma once

#include <yaml-cpp/yaml.h>

#include "io/parser/parse_data.hpp"
#include "tactile_def.hpp"

namespace tactile::parsing {

[[nodiscard]] auto parse_component_definitions(const YAML::Node& node, map_data& data)
    -> parse_error;

[[nodiscard]] auto parse_properties(const YAML::Node& node,
                                    attribute_context_data& context) -> parse_error;

[[nodiscard]] auto parse_components(const YAML::Node& node,
                                    const map_data& map,
                                    attribute_context_data& context) -> parse_error;

}  // namespace tactile::parsing
