#pragma once

#include <yaml-cpp/yaml.h>

#include "common/expected.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(const YAML::Node& node, int nRows, int nCols)
    -> Expected<TileLayerData, ParseError>;

}  // namespace Tactile::IO
