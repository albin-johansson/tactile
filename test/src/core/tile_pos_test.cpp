
/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/tile/tile_pos.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

TEST(TilePosition, Defaults)
{
  const TilePos position;
  ASSERT_EQ(0, position.row());
  ASSERT_EQ(0, position.col());
}

TEST(TilePosition, RowColConstructor)
{
  const TilePos a {123, 557};
  ASSERT_EQ(123, a.row());
  ASSERT_EQ(557, a.col());

  const TilePos b {-1, 0};
  ASSERT_EQ(-1, b.row());

  const TilePos c {0, -1};
  ASSERT_EQ(-1, c.col());
}

TEST(TilePosition, From)
{
  const auto position = TilePos::from(42u, 849u);
  ASSERT_EQ(42, position.row());
  ASSERT_EQ(849, position.col());
}

TEST(TilePosition, SetRow)
{
  TilePos position;

  const auto row = 892;
  position.set_row(row);

  ASSERT_EQ(row, position.row());
}

TEST(TilePosition, SetCol)
{
  TilePos position;

  const auto column = 438'291;
  position.set_col(column);

  ASSERT_EQ(column, position.col());
}

TEST(TilePosition, OffsetBy)
{
  const TilePos position {3, 4};
  const auto result = position.offset_by(4, 6);
  ASSERT_EQ(7, result.row());
  ASSERT_EQ(10, result.col());
}

TEST(TilePosition, Row)
{
  const TilePos position {8'324, 0};
  ASSERT_EQ(8'324, position.row());
}

TEST(TilePosition, Col)
{
  const TilePos position {0, 493};
  ASSERT_EQ(493, position.col());
}

TEST(TilePosition, URow)
{
  const TilePos position {6'532, 0};
  ASSERT_EQ(6'532u, position.urow());
}

TEST(TilePosition, UCol)
{
  const TilePos position {0, 18'343};
  ASSERT_EQ(18'343u, position.ucol());
}

TEST(TilePosition, RowToY)
{
  const auto tile_size = 134;
  const TilePos position {12, 34};
  ASSERT_EQ(12 * tile_size, position.row_to_y(tile_size));
}

TEST(TilePosition, ColToX)
{
  const auto tile_size = 68;
  const TilePos position {12, 34};
  ASSERT_EQ(34 * tile_size, position.col_to_x(tile_size));
}

TEST(TilePosition, North)
{
  const TilePos source {7, 15};
  const TilePos moved = source.north();

  ASSERT_EQ(source.row() - 1, moved.row());
  ASSERT_EQ(source.col(), moved.col());
}

TEST(TilePosition, East)
{
  const TilePos source {52, 77};
  const TilePos moved = source.east();

  ASSERT_EQ(source.row(), moved.row());
  ASSERT_EQ(source.col() + 1, moved.col());
}

TEST(TilePosition, South)
{
  const TilePos source {33, 6};
  const TilePos moved = source.south();

  ASSERT_EQ(source.row() + 1, moved.row());
  ASSERT_EQ(source.col(), moved.col());
}

TEST(TilePosition, West)
{
  const TilePos source {62, 39};
  const TilePos moved = source.west();

  ASSERT_EQ(source.row(), moved.row());
  ASSERT_EQ(source.col() - 1, moved.col());
}

TEST(TilePosition, Addition)
{
  const TilePos a {2, 3};
  const TilePos b {6, 4};

  const auto sum = a + b;
  ASSERT_EQ(a.row() + b.row(), sum.row());
  ASSERT_EQ(a.col() + b.col(), sum.col());
}

TEST(TilePosition, Subtraction)
{
  const TilePos a {13, 7};
  const TilePos b {4, 2};

  const auto diff = a - b;
  ASSERT_EQ(a.row() - b.row(), diff.row());
  ASSERT_EQ(a.col() - b.col(), diff.col());
}

TEST(TilePosition, AsVec2)
{
  const TilePos pos {832, 234};
  const auto vec = pos.as_vec2();

  ASSERT_EQ(pos.row(), vec.y);
  ASSERT_EQ(pos.col(), vec.x);
}

TEST(TilePosition, AsVec2f)
{
  const TilePos pos {-325, 834};
  const auto vec = pos.as_vec2f();

  ASSERT_EQ(static_cast<float>(pos.row()), vec.y);
  ASSERT_EQ(static_cast<float>(pos.col()), vec.x);
}

TEST(TilePosition, EqualityOperator)
{
  const TilePos pos {45, 23};
  ASSERT_EQ(pos, pos);

  const TilePos copy {pos};
  ASSERT_EQ(pos, copy);
  ASSERT_EQ(copy, pos);

  const TilePos diff {32, 93};
  ASSERT_FALSE(pos == diff);
  ASSERT_FALSE(diff == pos);
}

TEST(TilePosition, InequalityOperator)
{
  const TilePos pos {45, 23};
  ASSERT_FALSE(pos != pos);

  const TilePos copy {pos};
  ASSERT_FALSE(pos != copy);
  ASSERT_FALSE(copy != pos);

  const TilePos other {839, 54};
  ASSERT_NE(pos, other);
  ASSERT_NE(other, pos);
}

}  // namespace tactile::test
