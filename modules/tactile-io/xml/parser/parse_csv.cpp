#include "parse_csv.hpp"

#include <tactile_stdlib.hpp>

#include "io_utils.hpp"

namespace tactile::IO {

auto ParseCSV(const CStr csv, TileLayer& layer) -> ParseError
{
  const auto nCols = GetColumnCount(layer);

  usize index{};
  for (const auto& token : Split(csv, ',')) {
    if (const auto id = FromString<int32>(token.c_str())) {
      const auto [row, col] = ToMatrixCoords(index, nCols);
      SetTile(layer, row, col, *id);

      ++index;
    }
    else {
      return ParseError::CouldNotParseTiles;
    }
  }

  return ParseError::None;
}

}  // namespace tactile::IO
