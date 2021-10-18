#pragma once

#include <vector>  // vector

#include <tactile/io/maps/parse_error.hpp>
#include <tactile/io/maps/parse_ir.hpp>

#include "common.hpp"

#include <yaml-cpp/yaml.h>

namespace Tactile::IO {

[[nodiscard]] auto ParseProperties(const YAML::Node& node)
    -> Expected<std::vector<PropertyData>, ParseError>;

}  // namespace Tactile::IO