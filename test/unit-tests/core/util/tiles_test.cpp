#include "core/util/tiles.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

TEST(TileUtils, ToMatrixCoords)
{
  ASSERT_EQ(0, to_matrix_coords(0, 3).row);
  ASSERT_EQ(0, to_matrix_coords(0, 3).col);

  ASSERT_EQ(0, to_matrix_coords(1, 3).row);
  ASSERT_EQ(1, to_matrix_coords(1, 3).col);

  ASSERT_EQ(0, to_matrix_coords(2, 3).row);
  ASSERT_EQ(2, to_matrix_coords(2, 3).col);

  ASSERT_EQ(1, to_matrix_coords(3, 3).row);
  ASSERT_EQ(0, to_matrix_coords(3, 3).col);

  ASSERT_EQ(1, to_matrix_coords(4, 3).row);
  ASSERT_EQ(1, to_matrix_coords(4, 3).col);

  ASSERT_EQ(1, to_matrix_coords(5, 3).row);
  ASSERT_EQ(2, to_matrix_coords(5, 3).col);
}

TEST(TileUtils, MakeTileRow)
{
  const auto row = make_tile_row(10);
  ASSERT_EQ(10, row.capacity());
  ASSERT_EQ(10, row.size());
}

TEST(TileUtils, MakeTileMatrix)
{
  const auto matrix = make_tile_matrix(5, 7);
  ASSERT_EQ(5, matrix.capacity());
  ASSERT_EQ(5, matrix.size());
  ASSERT_EQ(7, matrix.at(0).capacity());
  ASSERT_EQ(7, matrix.at(0).size());
}

}  // namespace tactile::test
