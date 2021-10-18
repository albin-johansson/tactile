#pragma once

#include <vector>  // vector

#include <tactile/io/maps/parse_error.hpp>
#include <tactile/io/maps/parse_ir.hpp>

#include "common.hpp"

#include <yaml-cpp/yaml.h>

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(const YAML::Node& seq, int nRows, int nCols)
    -> Expected<std::vector<LayerData>, ParseError>;

}  // namespace Tactile::IO
