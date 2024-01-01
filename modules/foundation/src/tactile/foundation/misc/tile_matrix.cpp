// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/misc/tile_matrix.hpp"

#include "tactile/foundation/debug/assert.hpp"
#include "tactile/foundation/misc/integer_conversion.hpp"

namespace tactile {

auto make_tile_row(const ssize column_count) -> TileRow
{
  TileRow row(as_unsigned(column_count), kEmptyTile);

  TACTILE_ASSERT(std::ssize(row) == column_count);

  return row;
}

auto make_tile_matrix(const MatrixExtent& extent) -> TileMatrix
{
  TileMatrix tile_matrix(as_unsigned(extent.row_count), make_tile_row(extent.col_count));

  TACTILE_ASSERT(std::ssize(tile_matrix) == extent.row_count);
  TACTILE_ASSERT(std::ssize(tile_matrix.at(0)) == extent.col_count);

  return tile_matrix;
}

}  // namespace tactile