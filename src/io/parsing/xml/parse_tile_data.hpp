#pragma once

#include <pugixml.hpp>  // xml_node

#include "common/ints.hpp"
#include "common/tile_matrix.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileData(pugi::xml_node node, int32 nCols, TileMatrix& matrix)
    -> ParseError;

}  // namespace Tactile::IO
