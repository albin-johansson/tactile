#pragma once

#include "ir.hpp"
#include "parse_error.hpp"
#include "yaml/yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseFancyTiles(const YAML::Node& node, Tileset& tileset)
    -> ParseError;

}  // namespace Tactile::IO
