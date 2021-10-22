#include "parse_tile_data.hpp"

#include <string>  // string

#include <tactile-base/map_position.hpp>

namespace Tactile::IO {

auto ParseTileData(const JSON& json, TileLayer& layer) -> ParseError
{
  // For now, only support the CSV tile encoding, which is the implicit default
  if (const auto it = json.find("encoding"); it != json.end()) {
    if (it->get<std::string>() != "csv") {
      return ParseError::UnsupportedTileEncoding;
    }
  }

  const auto data = json.at("data");
  if (!data.is_array()) {
    return ParseError::CouldNotParseTiles;
  }

  const auto nCols = GetColumnCount(layer);
  int32 index = 0;

  for (const auto& elem : data.items()) {
    const auto& value = elem.value();

    if (value.is_number_integer()) {
      const auto id = value.get<int32>();
      const auto pos = MapPosition::FromIndex(index, nCols);
      SetTile(layer, pos.GetRow(), pos.GetColumn(), id);
    }
    else {
      return ParseError::CouldNotParseTiles;
    }

    ++index;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
