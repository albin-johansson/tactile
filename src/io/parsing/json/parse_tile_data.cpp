#include "parse_tile_data.hpp"

#include "core/map/map_position.hpp"

namespace Tactile::IO {

auto ParseTileData(const JSON& json, const col_t nCols, TileMatrix& matrix)
    -> ParseError
{
  // For now, only support the CSV tile encoding, which is the implicit default
  if (const auto it = json.find("encoding"); it != json.end())
  {
    if (it->get<std::string>() != "csv")
    {
      return ParseError::UnsupportedTileEncoding;
    }
  }

  const auto data = json.at("data");
  if (!data.is_array())
  {
    return ParseError::CouldNotParseTiles;
  }

  int index = 0;
  for (const auto& elem : data.items())
  {
    const auto value = elem.value();
    if (value.is_number_integer())
    {
      const auto id = tile_id{value.get<int>()};
      const auto pos = MapPosition::FromIndex(index, nCols);
      matrix.at(pos.GetRowIndex()).at(pos.GetColumnIndex()) = id;
    }
    else
    {
      return ParseError::CouldNotParseTiles;
    }

    ++index;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
