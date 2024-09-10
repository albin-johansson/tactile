// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/base/util/tile_matrix.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

// tactile::make_tile_matrix
TEST(TileMatrix, MakeTileMatrix)
{
  const MatrixExtent extent {3, 4};
  const auto tile_matrix = make_tile_matrix(extent);

  ASSERT_EQ(tile_matrix.size(), extent.rows);
  EXPECT_EQ(tile_matrix.at(0).size(), extent.cols);
  EXPECT_EQ(tile_matrix.at(1).size(), extent.cols);
  EXPECT_EQ(tile_matrix.at(2).size(), extent.cols);

  for (std::ptrdiff_t row = 0; row < extent.rows; ++row) {
    for (std::ptrdiff_t col = 0; col < extent.cols; ++col) {
      EXPECT_EQ(tile_matrix.at(static_cast<std::size_t>(row)).at(static_cast<std::size_t>(col)),
                kEmptyTile);
    }
  }
}

}  // namespace tactile::test
