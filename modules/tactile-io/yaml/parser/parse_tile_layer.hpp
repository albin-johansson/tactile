#pragma once

#include <expected.hpp>  // expected

#include "parse_error.hpp"
#include "parse_ir.hpp"
#include "yaml/yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(const YAML::Node& node, int nRows, int nCols)
    -> tl::expected<TileLayerData, ParseError>;

}  // namespace Tactile::IO
