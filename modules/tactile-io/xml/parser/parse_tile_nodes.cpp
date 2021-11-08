#include "parse_tile_nodes.hpp"

#include <tactile_def.hpp>
#include <tactile_stdlib.hpp>

namespace Tactile::IO {

auto ParseTileNodes(const pugi::xml_node& data, TileLayer& layer) -> ParseError
{
  const auto nCols = IO::GetColumnCount(layer);

  usize index = 0;
  for (const auto node : data.children("tile")) {
    const auto id = node.attribute("gid").as_int(0);

    const auto [row, col] = ToMatrixCoords(index, nCols);
    IO::SetTile(layer, row, col, id);

    ++index;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
