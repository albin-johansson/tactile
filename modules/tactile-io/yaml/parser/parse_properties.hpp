#pragma once

#include "parse_error.hpp"
#include "tactile_io.hpp"
#include "yaml/yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseProperties(const YAML::Node& node, Map& map) -> ParseError;

[[nodiscard]] auto ParseProperties(const YAML::Node& node, Tileset& tileset)
    -> ParseError;

[[nodiscard]] auto ParseProperties(const YAML::Node& node, Tile& tile) -> ParseError;

[[nodiscard]] auto ParseProperties(const YAML::Node& node, Layer& layer) -> ParseError;

[[nodiscard]] auto ParseProperties(const YAML::Node& node, Object& object) -> ParseError;

}  // namespace Tactile::IO