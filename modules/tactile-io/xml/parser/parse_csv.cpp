#include "parse_csv.hpp"

#include <tactile_stdlib.hpp>

#include "string_utils.hpp"

namespace Tactile::IO {

auto ParseCSV(const CStr csv, TileLayer& layer) -> ParseError
{
  const auto nCols = IO::GetColumnCount(layer);

  usize index{};
  for (const auto& token : Split(csv, ',')) {
    if (const auto id = FromString<int32>(token.c_str())) {
      const auto [row, col] = ToMatrixCoords(index, nCols);
      IO::SetTile(layer, row, col, *id);

      ++index;
    }
    else {
      return ParseError::CouldNotParseTiles;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
