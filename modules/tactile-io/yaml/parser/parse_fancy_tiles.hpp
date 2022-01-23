#pragma once

#include "parse_error.hpp"
#include "tactile_io.hpp"
#include "yaml/yaml_fwd.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseFancyTiles(const YAML::Node& node,
                                   const Map& map,
                                   Tileset& tileset) -> ParseError;

}  // namespace tactile::IO
