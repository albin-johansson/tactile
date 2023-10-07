// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/misc/tile_matrix.hpp"

#include <gtest/gtest.h>

using namespace tactile;

/// \tests tactile::make_tile_row
TEST(TileMatrix, MakeTileRow)
{
  const usize column_count = 7;

  const auto tile_row = make_tile_row(column_count);
  ASSERT_EQ(tile_row.size(), column_count);

  for (const auto tile_id : tile_row) {
    EXPECT_EQ(tile_id, kEmptyTile);
  }
}

/// \tests tactile::make_tile_matrix
TEST(TileMatrix, MakeTileMatrix)
{
  const MatrixExtent extent {15, 12};

  const auto tile_matrix = make_tile_matrix(extent);
  ASSERT_EQ(tile_matrix.size(), extent.row_count);

  for (const auto& tile_row : tile_matrix) {
    ASSERT_EQ(tile_row.size(), extent.col_count);

    for (const auto tile_id : tile_row) {
      EXPECT_EQ(tile_id, kEmptyTile);
    }
  }
}
