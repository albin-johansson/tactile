#pragma once

#include <pugixml.hpp>

#include <tactile/io/maps/parse_error.hpp>

#include "common/ints.hpp"
#include "common/tile_matrix.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileNodes(const pugi::xml_node& data,
                                  int32 nCols,
                                  TileMatrix& matrix) -> ParseError;

}  // namespace Tactile::IO
