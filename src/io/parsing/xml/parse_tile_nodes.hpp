#pragma once

#include <pugixml.hpp>

#include "aliases/ints.hpp"
#include "aliases/tile_matrix.hpp"
#include "io/parsing/parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileNodes(const pugi::xml_node& data,
                                  int32 nCols,
                                  TileMatrix& matrix) -> ParseError;

}  // namespace Tactile::IO
