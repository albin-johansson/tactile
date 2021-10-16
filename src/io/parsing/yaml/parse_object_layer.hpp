#pragma once

#include <yaml-cpp/yaml.h>

#include "common/expected.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const YAML::Node& node)
    -> Expected<ObjectLayerData, ParseError>;

}  // namespace Tactile::IO
