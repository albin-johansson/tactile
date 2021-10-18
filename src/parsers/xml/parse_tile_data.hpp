#pragma once

#include <pugixml.hpp>  // xml_node

#include <tactile-io/parse_error.hpp>
#include <tactile-io/parse_ir.hpp>

#include "common/ints.hpp"
#include "common/tile_matrix.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileData(pugi::xml_node node, int32 nCols, TileMatrix& matrix)
    -> ParseError;

}  // namespace Tactile::IO
