#pragma once

#include <pugixml.hpp>  // xml_node
#include <tactile-base/tactile_std.hpp>

#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileData(pugi::xml_node node, int32 nCols, TileMatrix& matrix)
    -> ParseError;

}  // namespace Tactile::IO
