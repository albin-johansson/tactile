#pragma once

#include <yaml-cpp/yaml.h>

#include "io/parser/parse_data.hpp"
#include "tactile_def.hpp"

namespace tactile::parsing {

[[nodiscard]] auto parse_object(const YAML::Node& node,
                                const map_data& map,
                                object_data* object) -> parse_error;

[[nodiscard]] auto parse_layers(const YAML::Node& sequence, map_data& data)
    -> parse_error;

}  // namespace tactile::parsing