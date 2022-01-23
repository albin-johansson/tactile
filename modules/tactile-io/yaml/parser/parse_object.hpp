#pragma once

#include "../yaml_fwd.hpp"
#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseObject(const YAML::Node& node, const Map& map, ObjectLayer& layer)
    -> ParseError;

[[nodiscard]] auto ParseObject(const YAML::Node& node, const Map& map, Tile& tile)
    -> ParseError;

}  // namespace tactile::IO