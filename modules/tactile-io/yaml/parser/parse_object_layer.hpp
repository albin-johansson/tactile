#pragma once

#include "ir.hpp"
#include "parse_error.hpp"
#include "yaml/yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const YAML::Node& node, Layer& layer) -> ParseError;

}  // namespace Tactile::IO
