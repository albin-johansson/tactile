
/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "common/tile_pos.hpp"

#include <doctest/doctest.h>

using namespace tactile;

TEST_SUITE("TilePos")
{
  TEST_CASE("Defaults")
  {
    const TilePos position;
    CHECK(position.row() == 0);
    CHECK(position.col() == 0);
  }

  TEST_CASE("row/column constructor")
  {
    const TilePos a {123, 557};
    CHECK(a.row() == 123);
    CHECK(a.col() == 557);

    const TilePos b {-1, 0};
    CHECK(b.row() == -1);

    const TilePos c {0, -1};
    CHECK(c.col() == -1);
  }

  TEST_CASE("from")
  {
    const auto position = TilePos::from(42u, 849u);
    CHECK(position.row() == 42);
    CHECK(position.col() == 849);
  }

  TEST_CASE("set_row")
  {
    TilePos position;

    const auto row = 892;
    position.set_row(row);

    CHECK(position.row() == row);
  }

  TEST_CASE("set_col")
  {
    TilePos position;

    const auto column = 438'291;
    position.set_col(column);

    CHECK(position.col() == column);
  }

  TEST_CASE("offset_by")
  {
    const TilePos position {3, 4};
    const auto result = position.offset_by(4, 6);
    CHECK(result.row() == 7);
    CHECK(result.col() == 10);
  }

  TEST_CASE("row")
  {
    const TilePos position {8'324, 0};
    CHECK(position.row() == 8'324);
  }

  TEST_CASE("col")
  {
    const TilePos position {0, 493};
    CHECK(position.col() == 493);
  }

  TEST_CASE("row_to_y")
  {
    const auto tile_size = 134;
    const TilePos position {12, 34};
    CHECK(position.row_to_y(tile_size) == 12 * tile_size);
  }

  TEST_CASE("col_to_x")
  {
    const auto tile_size = 68;
    const TilePos position {12, 34};
    CHECK(position.col_to_x(tile_size) == 34 * tile_size);
  }

  TEST_CASE("north")
  {
    const TilePos source {7, 15};
    const TilePos moved = source.north();

    CHECK(moved.row() == source.row() - 1);
    CHECK(moved.col() == source.col());
  }

  TEST_CASE("east")
  {
    const TilePos source {52, 77};
    const TilePos moved = source.east();

    CHECK(moved.row() == source.row());
    CHECK(moved.col() == source.col() + 1);
  }

  TEST_CASE("south")
  {
    const TilePos source {33, 6};
    const TilePos moved = source.south();

    CHECK(moved.row() == source.row() + 1);
    CHECK(moved.col() == source.col());
  }

  TEST_CASE("west")
  {
    const TilePos source {62, 39};
    const TilePos moved = source.west();

    CHECK(moved.row() == source.row());
    CHECK(moved.col() == source.col() - 1);
  }

  TEST_CASE("operator+")
  {
    const TilePos a {2, 3};
    const TilePos b {6, 4};

    const auto sum = a + b;
    CHECK(sum.row() == a.row() + b.row());
    CHECK(sum.col() == a.col() + b.col());
  }

  TEST_CASE("operator-")
  {
    const TilePos a {13, 7};
    const TilePos b {4, 2};

    const auto diff = a - b;
    CHECK(diff.row() == a.row() - b.row());
    CHECK(diff.col() == a.col() - b.col());
  }

  TEST_CASE("as_vec2")
  {
    const TilePos pos {832, 234};
    const auto vec = pos.as_vec2();

    CHECK(pos.row() == vec.y);
    CHECK(pos.col() == vec.x);
  }

  TEST_CASE("as_vec2f")
  {
    const TilePos pos {-325, 834};
    const auto vec = pos.as_vec2f();

    CHECK(vec.y == static_cast<float>(pos.row()));
    CHECK(vec.x == static_cast<float>(pos.col()));
  }

  TEST_CASE("operator==")
  {
    const TilePos pos {45, 23};
    CHECK(pos == pos);

    const TilePos copy {pos};
    CHECK(pos == copy);
    CHECK(copy == pos);

    const TilePos diff {32, 93};
    CHECK_FALSE(pos == diff);
    CHECK_FALSE(diff == pos);
  }

  TEST_CASE("operator!=")
  {
    const TilePos pos {45, 23};
    CHECK_FALSE(pos != pos);

    const TilePos copy {pos};
    CHECK_FALSE(pos != copy);
    CHECK_FALSE(copy != pos);

    const TilePos other {839, 54};
    CHECK(pos != other);
    CHECK(other != pos);
  }
}
