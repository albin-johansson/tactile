#include "core/map_position.hpp"

#include <gtest/gtest.h>

using namespace Tactile;

TEST(MapPosition, Defaults)
{
  const MapPosition position;
  ASSERT_EQ(0_row, position.GetRow());
  ASSERT_EQ(0_col, position.GetColumn());
}

TEST(Position, RowColConstructor)
{
  const MapPosition a{123_row, 557_col};
  ASSERT_EQ(123_row, a.GetRow());
  ASSERT_EQ(557_col, a.GetColumn());

  const MapPosition b{-1_row, 0_col};
  ASSERT_EQ(-1_row, b.GetRow());

  const MapPosition c{0_row, -1_col};
  ASSERT_EQ(-1_col, c.GetColumn());
}

TEST(Position, SetRow)
{
  MapPosition position;

  const auto row = 892_row;
  position.SetRow(row);

  ASSERT_EQ(row, position.GetRow());
}

TEST(Position, SetColumn)
{
  MapPosition position;

  const auto column = 438'291_col;
  position.SetColumn(column);

  ASSERT_EQ(column, position.GetColumn());
}

TEST(Position, OffsetBy)
{
  const MapPosition position{3_row, 4_col};
  const auto result = position.OffsetBy(4_row, 6_col);
  ASSERT_EQ(7_row, result.GetRow());
  ASSERT_EQ(10_col, result.GetColumn());
}

TEST(Position, GetRow)
{
  const MapPosition position{8'324_row, 0_col};
  ASSERT_EQ(8'324_row, position.GetRow());
}

TEST(Position, GetColumn)
{
  const MapPosition position{0_row, 493_col};
  ASSERT_EQ(493_col, position.GetColumn());
}

TEST(Position, GetRowIndex)
{
  const MapPosition position{6'532_row, 0_col};
  ASSERT_EQ(6'532u, position.GetRowIndex());
}

TEST(Position, GetColIndex)
{
  const MapPosition position{0_row, 18'343_col};
  ASSERT_EQ(18'343u, position.GetColumnIndex());
}

TEST(Position, RowToY)
{
  const auto tileSize = 134;
  const MapPosition position{12_row, 34_col};
  ASSERT_EQ(12 * tileSize, position.RowToY(tileSize));
}

TEST(Position, ColToX)
{
  const auto tileSize = 68;
  const MapPosition position{12_row, 34_col};
  ASSERT_EQ(34 * tileSize, position.ColumnToX(tileSize));
}

TEST(Position, North)
{
  const MapPosition source{7_row, 15_col};
  const MapPosition moved = source.North();

  ASSERT_EQ(source.GetRow() - 1_row, moved.GetRow());
  ASSERT_EQ(source.GetColumn(), moved.GetColumn());
}

TEST(Position, East)
{
  const MapPosition source{52_row, 77_col};
  const MapPosition moved = source.East();

  ASSERT_EQ(source.GetRow(), moved.GetRow());
  ASSERT_EQ(source.GetColumn() + 1_col, moved.GetColumn());
}

TEST(Position, South)
{
  const MapPosition source{33_row, 6_col};
  const MapPosition moved = source.South();

  ASSERT_EQ(source.GetRow() + 1_row, moved.GetRow());
  ASSERT_EQ(source.GetColumn(), moved.GetColumn());
}

TEST(Position, West)
{
  const MapPosition source{62_row, 39_col};
  const MapPosition moved = source.West();

  ASSERT_EQ(source.GetRow(), moved.GetRow());
  ASSERT_EQ(source.GetColumn() - 1_col, moved.GetColumn());
}

TEST(Position, Addition)
{
  const MapPosition a{2_row, 3_col};
  const MapPosition b{6_row, 4_col};

  const auto sum = a + b;
  ASSERT_EQ(a.GetRow() + b.GetRow(), sum.GetRow());
  ASSERT_EQ(a.GetColumn() + b.GetColumn(), sum.GetColumn());
}

TEST(Position, Subtraction)
{
  const MapPosition a{13_row, 7_col};
  const MapPosition b{4_row, 2_col};

  const auto diff = a - b;
  ASSERT_EQ(a.GetRow() - b.GetRow(), diff.GetRow());
  ASSERT_EQ(a.GetColumn() - b.GetColumn(), diff.GetColumn());
}

TEST(Position, EqualityOperator)
{
  const MapPosition pos{45_row, 23_col};
  ASSERT_EQ(pos, pos);

  const MapPosition copy{pos};
  ASSERT_EQ(pos, copy);
  ASSERT_EQ(copy, pos);

  const MapPosition diff{32_row, 93_col};
  ASSERT_FALSE(pos == diff);
  ASSERT_FALSE(diff == pos);
}

TEST(Position, InequalityOperator)
{
  const MapPosition pos{45_row, 23_col};
  ASSERT_FALSE(pos != pos);

  const MapPosition copy{pos};
  ASSERT_FALSE(pos != copy);
  ASSERT_FALSE(copy != pos);

  const MapPosition other{839_row, 54_col};
  ASSERT_NE(pos, other);
  ASSERT_NE(other, pos);
}
