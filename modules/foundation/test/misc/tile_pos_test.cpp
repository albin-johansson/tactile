// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/misc/tile_pos.hpp"

#include <gtest/gtest.h>

using namespace tactile;
using namespace tactile::int_literals;

/// \tests tactile::TilePos::TilePos
TEST(TilePos, DefaultConstructor)
{
  const TilePos pos;
  EXPECT_EQ(pos.row, 0_z);
  EXPECT_EQ(pos.col, 0_z);
}

/// \tests tactile::operator+(const TilePos&, const TilePos&)
TEST(TilePos, AdditionOperator)
{
  const TilePos p1 {4, -7};
  const TilePos p2 {3, 5};
  const TilePos p3 = p1 + p2;

  EXPECT_EQ(p3.row, p1.row + p2.row);
  EXPECT_EQ(p3.col, p1.col + p2.col);
}

/// \tests tactile::operator-(const TilePos&, const TilePos&)
TEST(TilePos, SubtractionOperator)
{
  const TilePos p1 {-82, 190};
  const TilePos p2 {-46, -5};
  const TilePos p3 = p1 - p2;

  EXPECT_EQ(p3.row, p1.row - p2.row);
  EXPECT_EQ(p3.col, p1.col - p2.col);
}

/// \tests tactile::operator+=(TilePos&, const TilePos&)
TEST(TilePos, AdditionAssignmentOperator)
{
  const TilePos p1_initial {63, 46};

  TilePos p1 {p1_initial};
  const TilePos p2 {12, -91};

  // Perform the assignment and make sure that the returned reference is correct.
  EXPECT_EQ(&(p1 += p2), &p1);

  EXPECT_EQ(p1.row, p1_initial.row + p2.row);
  EXPECT_EQ(p1.col, p1_initial.col + p2.col);
}

/// \tests tactile::operator-=(TilePos&, const TilePos&)
TEST(TilePos, SubtractionAssignmentOperator)
{
  const TilePos p1_initial {-812, 712};

  TilePos p1 {p1_initial};
  const TilePos p2 {412, 216};

  // Perform the assignment and make sure that the returned reference is correct.
  EXPECT_EQ(&(p1 -= p2), &p1);

  EXPECT_EQ(p1.row, p1_initial.row - p2.row);
  EXPECT_EQ(p1.col, p1_initial.col - p2.col);
}

/// \tests tactile::TilePos::operator<=>
TEST(TilePos, ComparisonOperators)
{
  const TilePos p1 {821, 611};
  const TilePos p2 {-123, 436};

  EXPECT_EQ(p1, p1);
  EXPECT_EQ(p2, p2);

  EXPECT_NE(p1, p2);

  EXPECT_GE(p1, p1);
  EXPECT_GE(p1, p2);
  EXPECT_GT(p1, p2);

  EXPECT_LE(p2, p2);
  EXPECT_LE(p2, p1);
  EXPECT_LT(p2, p1);
}