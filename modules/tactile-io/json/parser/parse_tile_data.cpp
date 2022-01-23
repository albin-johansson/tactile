#include "parse_tile_data.hpp"

#include <string>  // string

#include <tactile_stdlib.hpp>

namespace tactile::IO {

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
  usize index = 0;

  for (const auto& elem : data.items()) {
    const auto& value = elem.value();
    if (value.is_number_integer()) {
      const auto id = value.get<int32>();

      const auto [row, col] = ToMatrixCoords(index, nCols);
      SetTile(layer, row, col, id);

      ++index;
    }
    else {
      return ParseError::CouldNotParseTiles;
    }
  }

  return ParseError::None;
}

}  // namespace tactile::IO
