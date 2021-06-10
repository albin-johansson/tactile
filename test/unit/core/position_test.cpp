#include "position.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(Position, Defaults)
{
  const core::Position position;
  EXPECT_EQ(0_row, position.Row());
  EXPECT_EQ(0_col, position.Column());
}

TEST(Position, RowColConstructor)
{
  {  // Valid arguments
    const core::Position position{123_row, 557_col};
    EXPECT_EQ(123_row, position.Row());
    EXPECT_EQ(557_col, position.Column());
  }

  {  // Edge-case arguments
    const core::Position valid1{0_row, 0_col};
    EXPECT_EQ(0_row, valid1.Row());
    EXPECT_EQ(0_col, valid1.Column());

    const core::Position invalid1{-1_row, 0_col};
    EXPECT_EQ(0_row, invalid1.Row());

    const core::Position invalid2{0_row, -1_col};
    EXPECT_EQ(0_col, invalid2.Column());
  }
}

TEST(Position, SetRow)
{
  core::Position position;

  const auto row = 892_row;
  position.SetRow(row);

  EXPECT_EQ(row, position.Row());
}

TEST(Position, SetCol)
{
  core::Position position;

  const auto col = 438'291_col;
  position.SetColumn(col);

  EXPECT_EQ(col, position.Column());
}

TEST(Position, OffsetBy)
{
  const core::Position position;
  const auto result = position.OffsetBy(4_row, 6_col);
  EXPECT_EQ(4_row, result.Row());
  EXPECT_EQ(6_col, result.Column());
}

TEST(Position, Addition)
{
  const core::Position fst{2_row, 3_col};
  const core::Position snd{6_row, 4_col};

  const auto sum = fst + snd;
  EXPECT_EQ(fst.Row() + snd.Row(), sum.Row());
  EXPECT_EQ(fst.Column() + snd.Column(), sum.Column());
}

TEST(Position, Row)
{
  const core::Position position{8'324_row, 0_col};
  EXPECT_EQ(8'324_row, position.Row());
}

TEST(Position, Col)
{
  const core::Position position{0_row, 493_col};
  EXPECT_EQ(493_col, position.Column());
}

TEST(Position, RowIndex)
{
  const core::Position position{6'532_row, 0_col};
  EXPECT_EQ(6'532u, position.RowIndex());
}

TEST(Position, ColIndex)
{
  const core::Position position{0_row, 18'343_col};
  EXPECT_EQ(18'343u, position.ColumnIndex());
}

TEST(Position, RowToY)
{
  const auto tileSize = 134;
  const core::Position position{12_row, 34_col};
  EXPECT_EQ(12 * tileSize, position.RowToY(tileSize));
}

TEST(Position, ColToX)
{
  const auto tileSize = 68;
  const core::Position position{12_row, 34_col};
  EXPECT_EQ(34 * tileSize, position.ColumnToX(tileSize));
}

TEST(Position, Unpack)
{
  const core::Position position{32_row, 65_col};
  const auto [row, col] = position.Unpack();
  EXPECT_EQ(row, position.Row());
  EXPECT_EQ(col, position.Column());
}

TEST(Position, North)
{
  const core::Position source{7_row, 15_col};
  const core::Position moved = source.North();

  EXPECT_EQ(source.Row() - 1_row, moved.Row());
  EXPECT_EQ(source.Column(), moved.Column());
}

TEST(Position, East)
{
  const core::Position source{52_row, 77_col};
  const core::Position moved = source.East();

  EXPECT_EQ(source.Row(), moved.Row());
  EXPECT_EQ(source.Column() + 1_col, moved.Column());
}

TEST(Position, South)
{
  const core::Position source{33_row, 6_col};
  const core::Position moved = source.South();

  EXPECT_EQ(source.Row() + 1_row, moved.Row());
  EXPECT_EQ(source.Column(), moved.Column());
}

TEST(Position, West)
{
  const core::Position source{62_row, 39_col};
  const core::Position moved = source.West();

  EXPECT_EQ(source.Row(), moved.Row());
  EXPECT_EQ(source.Column() - 1_col, moved.Column());
}

TEST(Position, EqualityOperator)
{
  const core::Position pos{45_row, 23_col};
  EXPECT_EQ(pos, pos);

  const core::Position copy{pos};
  EXPECT_EQ(pos, copy);
  EXPECT_EQ(copy, pos);

  const core::Position diff{32_row, 93_col};
  EXPECT_FALSE(pos == diff);
  EXPECT_FALSE(diff == pos);
}

TEST(Position, InequalityOperator)
{
  const core::Position pos{45_row, 23_col};
  EXPECT_FALSE(pos != pos);

  const core::Position copy{pos};
  EXPECT_FALSE(pos != copy);
  EXPECT_FALSE(copy != pos);

  const core::Position other{839_row, 54_col};
  EXPECT_NE(pos, other);
  EXPECT_NE(other, pos);
}