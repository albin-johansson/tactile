#pragma once

#include "parse_error.hpp"
#include "tactile_io.hpp"
#include "yaml/yaml_fwd.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseLayers(const YAML::Node& seq, Map& map) -> ParseError;

}  // namespace tactile::IO
