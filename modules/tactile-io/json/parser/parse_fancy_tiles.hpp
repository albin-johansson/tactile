#pragma once

#include <vector>  // vector

#include <expected.hpp>  // expected

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseFancyTiles(const JSON& json)
    -> tl::expected<std::vector<TileData>, ParseError>;

}  // namespace Tactile::IO
