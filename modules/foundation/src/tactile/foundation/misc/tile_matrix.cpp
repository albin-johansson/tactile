// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/misc/tile_matrix.hpp"

#include "tactile/foundation/debug/assert.hpp"

namespace tactile {

auto make_tile_row(const usize column_count) -> TileRow
{
  TileRow row(column_count, kEmptyTile);

  TACTILE_ASSERT(row.size() == column_count);

  return row;
}

auto make_tile_matrix(const MatrixExtent& extent) -> TileMatrix
{
  TileMatrix tile_matrix(extent.row_count, make_tile_row(extent.col_count));

  TACTILE_ASSERT(tile_matrix.size() == extent.row_count);
  TACTILE_ASSERT(tile_matrix.at(0).size() == extent.col_count);

  return tile_matrix;
}

}  // namespace tactile