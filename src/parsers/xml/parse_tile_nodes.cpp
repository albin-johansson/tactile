#include "parse_tile_nodes.hpp"

#include "core/map_position.hpp"

namespace Tactile::IO {

auto ParseTileNodes(const pugi::xml_node& data, const int32 nCols, TileMatrix& matrix)
    -> ParseError
{
  int index = 0;
  for (const auto node : data.children("tile")) {
    const auto gid = node.attribute("gid").as_int(empty_tile);

    const auto pos = MapPosition::FromIndex(index, nCols);
    matrix.at(pos.GetRowIndex()).at(pos.GetColumnIndex()) = TileID{gid};

    ++index;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
