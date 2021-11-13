#pragma once

#include "parse_error.hpp"
#include "tactile_io.hpp"
#include "yaml/yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const YAML::Node& node, Layer& layer) -> ParseError;

}  // namespace Tactile::IO
