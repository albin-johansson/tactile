#pragma once

#include <tactile/io/maps/parse_error.hpp>
#include <tactile/io/maps/parse_ir.hpp>

#include "common.hpp"

#include <yaml-cpp/yaml.h>

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(const YAML::Node& node, int nRows, int nCols)
    -> Expected<TileLayerData, ParseError>;

}  // namespace Tactile::IO
