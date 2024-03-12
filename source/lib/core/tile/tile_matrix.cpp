// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tile_matrix.hpp"

namespace tactile {

auto make_tile_row(const usize column_count) -> TileRow
{
  TileRow row;
  row.reserve(column_count);
  row.assign(column_count, 0);
  return row;
}

auto make_tile_matrix(const TileExtent extent) -> TileMatrix
{
  TileMatrix tiles;
  tiles.reserve(extent.rows);
  tiles.assign(extent.rows, make_tile_row(extent.cols));
  return tiles;
}

}  // namespace tactile
