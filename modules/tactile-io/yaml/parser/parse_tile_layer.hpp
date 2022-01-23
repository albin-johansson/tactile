#pragma once

#include "parse_error.hpp"
#include "tactile_io.hpp"
#include "yaml/yaml_fwd.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseTileLayer(const YAML::Node& node,
                                  Layer& layer,
                                  usize nRows,
                                  usize nCols) -> ParseError;

}  // namespace tactile::IO
