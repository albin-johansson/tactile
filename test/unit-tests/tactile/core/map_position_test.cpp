
#include "core/tile_position.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(MapPosition, Defaults)
{
  const tile_position position;
  ASSERT_EQ(0, position.row());
  ASSERT_EQ(0, position.col());
}

TEST(MapPosition, RowColConstructor)
{
  const tile_position a{123, 557};
  ASSERT_EQ(123, a.row());
  ASSERT_EQ(557, a.col());

  const tile_position b{-1, 0};
  ASSERT_EQ(-1, b.row());

  const tile_position c{0, -1};
  ASSERT_EQ(-1, c.col());
}

TEST(MapPosition, From)
{
  const auto position = tile_position::from(42u, 849u);
  ASSERT_EQ(42, position.row());
  ASSERT_EQ(849, position.col());
}

TEST(MapPosition, SetRow)
{
  tile_position position;

  const auto row = 892;
  position.set_row(row);

  ASSERT_EQ(row, position.row());
}

TEST(MapPosition, SetColumn)
{
  tile_position position;

  const auto column = 438'291;
  position.set_col(column);

  ASSERT_EQ(column, position.col());
}

TEST(MapPosition, OffsetBy)
{
  const tile_position position{3, 4};
  const auto result = position.offset_by(4, 6);
  ASSERT_EQ(7, result.row());
  ASSERT_EQ(10, result.col());
}

TEST(MapPosition, GetRow)
{
  const tile_position position{8'324, 0};
  ASSERT_EQ(8'324, position.row());
}

TEST(MapPosition, GetColumn)
{
  const tile_position position{0, 493};
  ASSERT_EQ(493, position.col());
}

TEST(MapPosition, GetRowIndex)
{
  const tile_position position{6'532, 0};
  ASSERT_EQ(6'532u, position.row_index());
}

TEST(MapPosition, GetColIndex)
{
  const tile_position position{0, 18'343};
  ASSERT_EQ(18'343u, position.col_index());
}

TEST(MapPosition, RowToY)
{
  const auto tileSize = 134;
  const tile_position position{12, 34};
  ASSERT_EQ(12 * tileSize, position.row_to_y(tileSize));
}

TEST(MapPosition, ColToX)
{
  const auto tileSize = 68;
  const tile_position position{12, 34};
  ASSERT_EQ(34 * tileSize, position.col_to_x(tileSize));
}

TEST(MapPosition, North)
{
  const tile_position source{7, 15};
  const tile_position moved = source.north();

  ASSERT_EQ(source.row() - 1, moved.row());
  ASSERT_EQ(source.col(), moved.col());
}

TEST(MapPosition, East)
{
  const tile_position source{52, 77};
  const tile_position moved = source.east();

  ASSERT_EQ(source.row(), moved.row());
  ASSERT_EQ(source.col() + 1, moved.col());
}

TEST(MapPosition, South)
{
  const tile_position source{33, 6};
  const tile_position moved = source.south();

  ASSERT_EQ(source.row() + 1, moved.row());
  ASSERT_EQ(source.col(), moved.col());
}

TEST(MapPosition, West)
{
  const tile_position source{62, 39};
  const tile_position moved = source.west();

  ASSERT_EQ(source.row(), moved.row());
  ASSERT_EQ(source.col() - 1, moved.col());
}

TEST(MapPosition, Addition)
{
  const tile_position a{2, 3};
  const tile_position b{6, 4};

  const auto sum = a + b;
  ASSERT_EQ(a.row() + b.row(), sum.row());
  ASSERT_EQ(a.col() + b.col(), sum.col());
}

TEST(MapPosition, Subtraction)
{
  const tile_position a{13, 7};
  const tile_position b{4, 2};

  const auto diff = a - b;
  ASSERT_EQ(a.row() - b.row(), diff.row());
  ASSERT_EQ(a.col() - b.col(), diff.col());
}

TEST(MapPosition, EqualityOperator)
{
  const tile_position pos{45, 23};
  ASSERT_EQ(pos, pos);

  const tile_position copy{pos};
  ASSERT_EQ(pos, copy);
  ASSERT_EQ(copy, pos);

  const tile_position diff{32, 93};
  ASSERT_FALSE(pos == diff);
  ASSERT_FALSE(diff == pos);
}

TEST(MapPosition, InequalityOperator)
{
  const tile_position pos{45, 23};
  ASSERT_FALSE(pos != pos);

  const tile_position copy{pos};
  ASSERT_FALSE(pos != copy);
  ASSERT_FALSE(copy != pos);

  const tile_position other{839, 54};
  ASSERT_NE(pos, other);
  ASSERT_NE(other, pos);
}
