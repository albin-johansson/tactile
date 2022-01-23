#pragma once

#include "parse_error.hpp"
#include "tactile_io.hpp"
#include "yaml/yaml_fwd.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseComponentDefinitions(const YAML::Node& node, Map& map)
    -> ParseError;

[[nodiscard]] auto ParseComponents(const YAML::Node& node, Map& map) -> ParseError;

[[nodiscard]] auto ParseComponents(const Map& map,
                                   const YAML::Node& node,
                                   Tileset& tileset) -> ParseError;

[[nodiscard]] auto ParseComponents(const Map& map, const YAML::Node& node, Tile& tile)
    -> ParseError;

[[nodiscard]] auto ParseComponents(const Map& map, const YAML::Node& node, Layer& layer)
    -> ParseError;

[[nodiscard]] auto ParseComponents(const Map& map, const YAML::Node& node, Object& object)
    -> ParseError;

}  // namespace tactile::IO
