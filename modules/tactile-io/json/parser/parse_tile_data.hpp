#pragma once

#include <expected.hpp>  // expected

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileData(const JSON& json, int32 nRows, int32 nCols)
    -> tl::expected<TileMatrix, ParseError>;

}  // namespace Tactile::IO
