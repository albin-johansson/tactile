#pragma once

#include "ir.hpp"
#include "parse_error.hpp"
#include "yaml/yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTilesets(const YAML::Node& seq, Map& map) -> ParseError;

}  // namespace Tactile::IO
