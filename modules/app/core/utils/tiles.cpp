#include "tiles.hpp"

namespace tactile {

auto make_tile_row(const usize nCols) -> tile_row
{
  tile_row row;
  row.reserve(nCols);
  row.assign(nCols, 0);
  return row;
}

auto make_tile_matrix(const usize nRows, const usize nCols) -> tile_matrix
{
  tile_matrix tiles;
  tiles.reserve(nRows);
  tiles.assign(nRows, make_tile_row(nCols));
  return tiles;
}

}  // namespace tactile
