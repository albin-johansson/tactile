#pragma once

#include <yaml-cpp/yaml.h>

#include <vector>  // vector

#include "common.hpp"
#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseProperties(const YAML::Node& node)
    -> Expected<std::vector<PropertyData>, ParseError>;

}  // namespace Tactile::IO