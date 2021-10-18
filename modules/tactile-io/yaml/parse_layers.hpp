#pragma once

#include <vector>  // vector

#include <expected.hpp>  // expected

#include "../parse_error.hpp"
#include "../parse_ir.hpp"
#include "yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(const YAML::Node& seq, int nRows, int nCols)
    -> tl::expected<std::vector<LayerData>, ParseError>;

}  // namespace Tactile::IO
