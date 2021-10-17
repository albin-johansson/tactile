#pragma once

#include <yaml-cpp/yaml.h>

#include "common.hpp"
#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const YAML::Node& node)
    -> Expected<ObjectLayerData, ParseError>;

}  // namespace Tactile::IO
