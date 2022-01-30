#include "core/utils/tiles.hpp"

#include <gtest/gtest.h>

using namespace tactile;

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
