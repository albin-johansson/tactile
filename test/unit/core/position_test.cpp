#include "position.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(Position, Defaults)
{
  const core::position position;
  EXPECT_EQ(0_row, position.row());
  EXPECT_EQ(0_col, position.col());
}

TEST(Position, RowColConstructor)
{
  {  // Valid arguments
    const core::position position{123_row, 557_col};
    EXPECT_EQ(123_row, position.row());
    EXPECT_EQ(557_col, position.col());
  }

  {  // Edge-case arguments
    const core::position valid1{0_row, 0_col};
    EXPECT_EQ(0_row, valid1.row());
    EXPECT_EQ(0_col, valid1.col());

    const core::position invalid1{-1_row, 0_col};
    EXPECT_EQ(0_row, invalid1.row());

    const core::position invalid2{0_row, -1_col};
    EXPECT_EQ(0_col, invalid2.col());
  }
}

TEST(Position, SetRow)
{
  core::position position;

  const auto row = 892_row;
  position.set_row(row);

  EXPECT_EQ(row, position.row());
}

TEST(Position, SetCol)
{
  core::position position;

  const auto col = 438'291_col;
  position.set_col(col);

  EXPECT_EQ(col, position.col());
}

TEST(Position, OffsetBy)
{
  const core::position position;
  const auto result = position.offset_by(4_row, 6_col);
  EXPECT_EQ(4_row, result.row());
  EXPECT_EQ(6_col, result.col());
}

TEST(Position, Addition)
{
  const core::position fst{2_row, 3_col};
  const core::position snd{6_row, 4_col};

  const auto sum = fst + snd;
  EXPECT_EQ(fst.row() + snd.row(), sum.row());
  EXPECT_EQ(fst.col() + snd.col(), sum.col());
}

TEST(Position, Row)
{
  const core::position position{8'324_row, 0_col};
  EXPECT_EQ(8'324_row, position.row());
}

TEST(Position, Col)
{
  const core::position position{0_row, 493_col};
  EXPECT_EQ(493_col, position.col());
}

TEST(Position, RowIndex)
{
  const core::position position{6'532_row, 0_col};
  EXPECT_EQ(6'532u, position.row_index());
}

TEST(Position, ColIndex)
{
  const core::position position{0_row, 18'343_col};
  EXPECT_EQ(18'343u, position.col_index());
}

TEST(Position, RowToY)
{
  const auto tileSize = 134;
  const core::position position{12_row, 34_col};
  EXPECT_EQ(12 * tileSize, position.row_to_y(tileSize));
}

TEST(Position, ColToX)
{
  const auto tileSize = 68;
  const core::position position{12_row, 34_col};
  EXPECT_EQ(34 * tileSize, position.col_to_x(tileSize));
}

TEST(Position, Unpack)
{
  const core::position position{32_row, 65_col};
  const auto [row, col] = position.unpack();
  EXPECT_EQ(row, position.row());
  EXPECT_EQ(col, position.col());
}

TEST(Position, North)
{
  const core::position source{7_row, 15_col};
  const core::position moved = source.north();

  EXPECT_EQ(source.row() - 1_row, moved.row());
  EXPECT_EQ(source.col(), moved.col());
}

TEST(Position, East)
{
  const core::position source{52_row, 77_col};
  const core::position moved = source.east();

  EXPECT_EQ(source.row(), moved.row());
  EXPECT_EQ(source.col() + 1_col, moved.col());
}

TEST(Position, South)
{
  const core::position source{33_row, 6_col};
  const core::position moved = source.south();

  EXPECT_EQ(source.row() + 1_row, moved.row());
  EXPECT_EQ(source.col(), moved.col());
}

TEST(Position, West)
{
  const core::position source{62_row, 39_col};
  const core::position moved = source.west();

  EXPECT_EQ(source.row(), moved.row());
  EXPECT_EQ(source.col() - 1_col, moved.col());
}

TEST(Position, EqualityOperator)
{
  const core::position pos{45_row, 23_col};
  EXPECT_EQ(pos, pos);

  const core::position copy{pos};
  EXPECT_EQ(pos, copy);
  EXPECT_EQ(copy, pos);

  const core::position diff{32_row, 93_col};
  EXPECT_FALSE(pos == diff);
  EXPECT_FALSE(diff == pos);
}

TEST(Position, InequalityOperator)
{
  const core::position pos{45_row, 23_col};
  EXPECT_FALSE(pos != pos);

  const core::position copy{pos};
  EXPECT_FALSE(pos != copy);
  EXPECT_FALSE(copy != pos);

  const core::position other{839_row, 54_col};
  EXPECT_NE(pos, other);
  EXPECT_NE(other, pos);
}