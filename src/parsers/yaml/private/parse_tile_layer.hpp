#pragma once

#include <yaml-cpp/yaml.h>

#include "common.hpp"
#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(const YAML::Node& node, int nRows, int nCols)
    -> Expected<TileLayerData, ParseError>;

}  // namespace Tactile::IO
