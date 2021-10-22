#pragma once

#include "parse_error.hpp"
#include "parse_ir.hpp"
#include "yaml/yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(const YAML::Node& seq, Map& map) -> ParseError;

}  // namespace Tactile::IO
