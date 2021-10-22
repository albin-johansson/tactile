#pragma once

#include "../yaml_fwd.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObject(const YAML::Node& node, ObjectLayer& layer) -> ParseError;

[[nodiscard]] auto ParseObject(const YAML::Node& node, Tile& tile) -> ParseError;

}  // namespace Tactile::IO