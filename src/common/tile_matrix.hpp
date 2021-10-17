#pragma once

#include <vector>  // vector

#include "ints.hpp"
#include "tile_id.hpp"

namespace Tactile {

using TileRow = std::vector<TileID>;
using TileMatrix = std::vector<TileRow>;

[[nodiscard]] inline auto MakeTileRow(const int32 nCols, const TileID value) -> TileRow
{
  TileRow row;
  row.reserve(nCols);
  row.assign(nCols, value);
  return row;
}

[[nodiscard]] inline auto MakeTileMatrix(const int32 nRows, const int32 nCols)
    -> TileMatrix
{
  TileMatrix tiles;
  tiles.reserve(nRows);
  tiles.assign(nRows, MakeTileRow(nCols, empty_tile));
  return tiles;
}

}  // namespace Tactile
