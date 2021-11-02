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

    const auto pos = MapPosition::FromIndex(index, static_cast<int32>(nCols));
    IO::SetTile(layer, pos.GetRowIndex(), pos.GetColumnIndex(), id);

    ++index;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
