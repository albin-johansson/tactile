#pragma once

#include <pugixml.hpp>

#include "parse_error.hpp"

#include <tactile-base/tactile_std.hpp>

namespace Tactile::IO {

[[nodiscard]] auto ParseTileNodes(const pugi::xml_node& data,
                                  int32 nCols,
                                  TileMatrix& matrix) -> ParseError;

}  // namespace Tactile::IO
