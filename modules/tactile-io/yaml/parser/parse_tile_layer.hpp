#pragma once

#include "parse_error.hpp"
#include "parse_ir.hpp"
#include "yaml/yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(const YAML::Node& node,
                                  Layer& layer,
                                  int32 nRows,
                                  int32 nCols) -> ParseError;

}  // namespace Tactile::IO
