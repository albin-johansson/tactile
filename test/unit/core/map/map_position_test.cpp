#include "core/map_position.hpp"

#include <gtest/gtest.h>

using namespace Tactile;

TEST(MapPosition, Defaults)
{
  const MapPosition position;
  ASSERT_EQ(0, position.GetRow());
  ASSERT_EQ(0, position.GetColumn());
}

TEST(Position, RowColConstructor)
{
  const MapPosition a{123, 557};
  ASSERT_EQ(123, a.GetRow());
  ASSERT_EQ(557, a.GetColumn());

  const MapPosition b{-1, 0};
  ASSERT_EQ(-1, b.GetRow());

  const MapPosition c{0, -1};
  ASSERT_EQ(-1, c.GetColumn());
}

TEST(Position, SetRow)
{
  MapPosition position;

  const auto row = 892;
  position.SetRow(row);

  ASSERT_EQ(row, position.GetRow());
}

TEST(Position, SetColumn)
{
  MapPosition position;

  const auto column = 438'291;
  position.SetColumn(column);

  ASSERT_EQ(column, position.GetColumn());
}

TEST(Position, OffsetBy)
{
  const MapPosition position{3, 4};
  const auto result = position.OffsetBy(4, 6);
  ASSERT_EQ(7, result.GetRow());
  ASSERT_EQ(10, result.GetColumn());
}

TEST(Position, GetRow)
{
  const MapPosition position{8'324, 0};
  ASSERT_EQ(8'324, position.GetRow());
}

TEST(Position, GetColumn)
{
  const MapPosition position{0, 493};
  ASSERT_EQ(493, position.GetColumn());
}

TEST(Position, GetRowIndex)
{
  const MapPosition position{6'532, 0};
  ASSERT_EQ(6'532u, position.GetRowIndex());
}

TEST(Position, GetColIndex)
{
  const MapPosition position{0, 18'343};
  ASSERT_EQ(18'343u, position.GetColumnIndex());
}

TEST(Position, RowToY)
{
  const auto tileSize = 134;
  const MapPosition position{12, 34};
  ASSERT_EQ(12 * tileSize, position.RowToY(tileSize));
}

TEST(Position, ColToX)
{
  const auto tileSize = 68;
  const MapPosition position{12, 34};
  ASSERT_EQ(34 * tileSize, position.ColumnToX(tileSize));
}

TEST(Position, North)
{
  const MapPosition source{7, 15};
  const MapPosition moved = source.North();

  ASSERT_EQ(source.GetRow() - 1, moved.GetRow());
  ASSERT_EQ(source.GetColumn(), moved.GetColumn());
}

TEST(Position, East)
{
  const MapPosition source{52, 77};
  const MapPosition moved = source.East();

  ASSERT_EQ(source.GetRow(), moved.GetRow());
  ASSERT_EQ(source.GetColumn() + 1, moved.GetColumn());
}

TEST(Position, South)
{
  const MapPosition source{33, 6};
  const MapPosition moved = source.South();

  ASSERT_EQ(source.GetRow() + 1, moved.GetRow());
  ASSERT_EQ(source.GetColumn(), moved.GetColumn());
}

TEST(Position, West)
{
  const MapPosition source{62, 39};
  const MapPosition moved = source.West();

  ASSERT_EQ(source.GetRow(), moved.GetRow());
  ASSERT_EQ(source.GetColumn() - 1, moved.GetColumn());
}

TEST(Position, Addition)
{
  const MapPosition a{2, 3};
  const MapPosition b{6, 4};

  const auto sum = a + b;
  ASSERT_EQ(a.GetRow() + b.GetRow(), sum.GetRow());
  ASSERT_EQ(a.GetColumn() + b.GetColumn(), sum.GetColumn());
}

TEST(Position, Subtraction)
{
  const MapPosition a{13, 7};
  const MapPosition b{4, 2};

  const auto diff = a - b;
  ASSERT_EQ(a.GetRow() - b.GetRow(), diff.GetRow());
  ASSERT_EQ(a.GetColumn() - b.GetColumn(), diff.GetColumn());
}

TEST(Position, EqualityOperator)
{
  const MapPosition pos{45, 23};
  ASSERT_EQ(pos, pos);

  const MapPosition copy{pos};
  ASSERT_EQ(pos, copy);
  ASSERT_EQ(copy, pos);

  const MapPosition diff{32, 93};
  ASSERT_FALSE(pos == diff);
  ASSERT_FALSE(diff == pos);
}

TEST(Position, InequalityOperator)
{
  const MapPosition pos{45, 23};
  ASSERT_FALSE(pos != pos);

  const MapPosition copy{pos};
  ASSERT_FALSE(pos != copy);
  ASSERT_FALSE(copy != pos);

  const MapPosition other{839, 54};
  ASSERT_NE(pos, other);
  ASSERT_NE(other, pos);
}
