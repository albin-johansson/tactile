#pragma once

#include <tactile/io/maps/parse_error.hpp>
#include <tactile/io/maps/parse_ir.hpp>

#include "common.hpp"

#include <yaml-cpp/yaml.h>

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const YAML::Node& node)
    -> Expected<ObjectLayerData, ParseError>;

}  // namespace Tactile::IO
