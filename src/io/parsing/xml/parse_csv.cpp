#include "parse_csv.hpp"

#include <sstream>  // istringstream
#include <string>   // string, erase, getline
#include <vector>   // vector

#include "core/map_position.hpp"
#include "utils/from_string.hpp"
#include "utils/string_utils.hpp"

namespace Tactile::IO {

auto ParseCSV(const CStr csv, const int32 nCols, TileMatrix& matrix) -> ParseError
{
  int index{};
  for (const auto& token : Split(csv, ',')) {
    if (const auto id = FromString<TileID::value_type>(token)) {
      const auto pos = MapPosition::FromIndex(index, nCols);
      matrix.at(pos.GetRowIndex()).at(pos.GetColumnIndex()) = TileID{*id};
      ++index;
    }
    else {
      return ParseError::CouldNotParseTiles;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
