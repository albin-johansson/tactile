#include "parse_csv.hpp"

#include <tactile-base/from_string.hpp>
#include <tactile-base/map_position.hpp>
#include <tactile-base/string_utils.hpp>

namespace Tactile::IO {

auto ParseCSV(const CStr csv, TileLayer& layer) -> ParseError
{
  const auto nCols = IO::GetColumnCount(layer);

  int32 index{};
  for (const auto& token : Split(csv, ',')) {
    if (const auto id = FromString<int32>(token)) {
      const auto pos = MapPosition::FromIndex(index, static_cast<int32>(nCols));
      IO::SetTile(layer, pos.GetRowIndex(), pos.GetColumnIndex(), *id);

      ++index;
    }
    else {
      return ParseError::CouldNotParseTiles;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
