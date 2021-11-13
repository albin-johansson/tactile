#pragma once

#include "parse_error.hpp"
#include "tactile_io.hpp"
#include "yaml/yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseFancyTiles(const YAML::Node& node, Tileset& tileset)
    -> ParseError;

}  // namespace Tactile::IO
