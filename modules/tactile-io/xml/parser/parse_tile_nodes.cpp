#include "parse_tile_nodes.hpp"

#include <tactile_def.hpp>
#include <tactile_stdlib.hpp>

namespace tactile::IO {

auto ParseTileNodes(const pugi::xml_node& data, TileLayer& layer) -> ParseError
{
  const auto nCols = GetColumnCount(layer);

  usize index = 0;
  for (const auto node : data.children("tile")) {
    const auto id = node.attribute("gid").as_int(0);

    const auto [row, col] = ToMatrixCoords(index, nCols);
    SetTile(layer, row, col, id);

    ++index;
  }

  return ParseError::None;
}

}  // namespace tactile::IO
