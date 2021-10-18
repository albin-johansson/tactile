#pragma once

#include <vector>  // vector

#include "../../parse_error.hpp"
#include "../../parse_ir.hpp"
#include "../yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseFancyTiles(const YAML::Node& node)
    -> tl::expected<std::vector<TileData>, ParseError>;

}  // namespace Tactile::IO
