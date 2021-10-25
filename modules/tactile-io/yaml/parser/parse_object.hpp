#pragma once

#include "../yaml_fwd.hpp"
#include "ir.hpp"
#include "parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObject(const YAML::Node& node, ObjectLayer& layer) -> ParseError;

[[nodiscard]] auto ParseObject(const YAML::Node& node, Tile& tile) -> ParseError;

}  // namespace Tactile::IO