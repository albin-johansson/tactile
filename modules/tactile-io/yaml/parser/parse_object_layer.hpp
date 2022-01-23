#pragma once

#include "parse_error.hpp"
#include "tactile_io.hpp"
#include "yaml/yaml_fwd.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const YAML::Node& node, const Map& map, Layer& layer)
    -> ParseError;

}  // namespace tactile::IO
