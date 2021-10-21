#include "parse_tile_data.hpp"

#include <json.hpp>  // json
#include <tactile-base/map_position.hpp>

namespace Tactile::IO {

auto ParseTileData(const JSON& json, const int32 nRows, const int32 nCols)
    -> tl::expected<TileMatrix, ParseError>
{
  // For now, only support the CSV tile encoding, which is the implicit default
  if (const auto it = json.find("encoding"); it != json.end()) {
    if (it->get<std::string>() != "csv") {
      return tl::make_unexpected(ParseError::UnsupportedTileEncoding);
    }
  }

  const auto data = json.at("data");
  if (!data.is_array()) {
    return tl::make_unexpected(ParseError::CouldNotParseTiles);
  }

  auto matrix = MakeTileMatrix(nRows, nCols);

  int index = 0;
  for (const auto& elem : data.items()) {
    const auto value = elem.value();
    if (value.is_number_integer()) {
      const auto id = TileID{value.get<TileID::value_type>()};
      const auto pos = MapPosition::FromIndex(index, nCols);
      matrix.at(pos.GetRowIndex()).at(pos.GetColumnIndex()) = id;
    }
    else {
      return tl::make_unexpected(ParseError::CouldNotParseTiles);
    }

    ++index;
  }

  return matrix;
}

}  // namespace Tactile::IO
