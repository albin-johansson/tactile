#pragma once

#include <yaml-cpp/yaml.h>

#include <vector>  // vector

#include "common/expected.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseProperties(const YAML::Node& node)
    -> Expected<std::vector<PropertyData>, ParseError>;

}  // namespace Tactile::IO