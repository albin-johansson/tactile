#pragma once

#include <yaml-cpp/yaml.h>

#include <vector>  // vector

#include "common/expected.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(const YAML::Node& seq, int nRows, int nCols)
    -> Expected<std::vector<LayerData>, ParseError>;

}  // namespace Tactile::IO
