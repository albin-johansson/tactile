// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/base/util/matrix_index.hpp"

#include <format>  // format

#include <gtest/gtest.h>

namespace tactile {

TEST(MatrixIndex, Format)
{
  EXPECT_EQ(std::format("{}", MatrixIndex {1, 2}), "(1, 2)");
}

// tactile::make_matrix_index
TEST(MatrixIndex, MakeMatrixIndex)
{
  EXPECT_EQ(make_matrix_index(0, 1), (MatrixIndex {0, 0}));
  EXPECT_EQ(make_matrix_index(1, 1), (MatrixIndex {1, 0}));

  EXPECT_EQ(make_matrix_index(0, 4), (MatrixIndex {0, 0}));
  EXPECT_EQ(make_matrix_index(1, 4), (MatrixIndex {0, 1}));
  EXPECT_EQ(make_matrix_index(2, 4), (MatrixIndex {0, 2}));
  EXPECT_EQ(make_matrix_index(3, 4), (MatrixIndex {0, 3}));

  EXPECT_EQ(make_matrix_index(4, 4), (MatrixIndex {1, 0}));
  EXPECT_EQ(make_matrix_index(5, 4), (MatrixIndex {1, 1}));
  EXPECT_EQ(make_matrix_index(6, 4), (MatrixIndex {1, 2}));
  EXPECT_EQ(make_matrix_index(7, 4), (MatrixIndex {1, 3}));

  EXPECT_EQ(make_matrix_index(8, 4), (MatrixIndex {2, 0}));
  EXPECT_EQ(make_matrix_index(9, 4), (MatrixIndex {2, 1}));
  EXPECT_EQ(make_matrix_index(10, 4), (MatrixIndex {2, 2}));
  EXPECT_EQ(make_matrix_index(11, 4), (MatrixIndex {2, 3}));
}

}  // namespace tactile
