#include "parse_tile_nodes.hpp"

#include <tactile-base/map_position.hpp>
#include <tactile-base/tactile_std.hpp>

namespace Tactile::IO {

auto ParseTileNodes(const pugi::xml_node& data, TileLayer& layer) -> ParseError
{
  const auto nCols = IO::GetColumnCount(layer);

  int32 index = 0;
  for (const auto node : data.children("tile")) {
    const auto id = node.attribute("gid").as_int(empty_tile);

    const auto pos = MapPosition::FromIndex(index, nCols);
    IO::SetTile(layer, pos.GetRow(), pos.GetColumn(), id);

    ++index;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
