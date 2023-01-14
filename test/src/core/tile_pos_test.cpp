
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

#include <doctest/doctest.h>

namespace tactile::test {

TEST_SUITE("TilePos")
{
  TEST_CASE("Defaults")
  {
    const TilePos position;
    REQUIRE(0 == position.row());
    REQUIRE(0 == position.col());
  }

  TEST_CASE("row/column constructor")
  {
    const TilePos a {123, 557};
    REQUIRE(123 == a.row());
    REQUIRE(557 == a.col());

    const TilePos b {-1, 0};
    REQUIRE(-1 == b.row());

    const TilePos c {0, -1};
    REQUIRE(-1 == c.col());
  }

  TEST_CASE("from")
  {
    const auto position = TilePos::from(42u, 849u);
    REQUIRE(42 == position.row());
    REQUIRE(849 == position.col());
  }

  TEST_CASE("set_row")
  {
    TilePos position;

    const auto row = 892;
    position.set_row(row);

    REQUIRE(row == position.row());
  }

  TEST_CASE("set_col")
  {
    TilePos position;

    const auto column = 438'291;
    position.set_col(column);

    REQUIRE(column == position.col());
  }

  TEST_CASE("offset_by")
  {
    const TilePos position {3, 4};
    const auto result = position.offset_by(4, 6);
    REQUIRE(7 == result.row());
    REQUIRE(10 == result.col());
  }

  TEST_CASE("row")
  {
    const TilePos position {8'324, 0};
    REQUIRE(8'324 == position.row());
  }

  TEST_CASE("col")
  {
    const TilePos position {0, 493};
    REQUIRE(493 == position.col());
  }

  TEST_CASE("row_to_y")
  {
    const auto tile_size = 134;
    const TilePos position {12, 34};
    REQUIRE(12 * tile_size == position.row_to_y(tile_size));
  }

  TEST_CASE("col_to_x")
  {
    const auto tile_size = 68;
    const TilePos position {12, 34};
    REQUIRE(34 * tile_size == position.col_to_x(tile_size));
  }

  TEST_CASE("north")
  {
    const TilePos source {7, 15};
    const TilePos moved = source.north();

    REQUIRE(source.row() - 1 == moved.row());
    REQUIRE(source.col() == moved.col());
  }

  TEST_CASE("east")
  {
    const TilePos source {52, 77};
    const TilePos moved = source.east();

    REQUIRE(source.row() == moved.row());
    REQUIRE(source.col() + 1 == moved.col());
  }

  TEST_CASE("south")
  {
    const TilePos source {33, 6};
    const TilePos moved = source.south();

    REQUIRE(source.row() + 1 == moved.row());
    REQUIRE(source.col() == moved.col());
  }

  TEST_CASE("west")
  {
    const TilePos source {62, 39};
    const TilePos moved = source.west();

    REQUIRE(source.row() == moved.row());
    REQUIRE(source.col() - 1 == moved.col());
  }

  TEST_CASE("operator+")
  {
    const TilePos a {2, 3};
    const TilePos b {6, 4};

    const auto sum = a + b;
    REQUIRE(a.row() + b.row() == sum.row());
    REQUIRE(a.col() + b.col() == sum.col());
  }

  TEST_CASE("operator-")
  {
    const TilePos a {13, 7};
    const TilePos b {4, 2};

    const auto diff = a - b;
    REQUIRE(a.row() - b.row() == diff.row());
    REQUIRE(a.col() - b.col() == diff.col());
  }

  TEST_CASE("as_vec2")
  {
    const TilePos pos {832, 234};
    const auto vec = pos.as_vec2();

    REQUIRE(pos.row() == vec.y);
    REQUIRE(pos.col() == vec.x);
  }

  TEST_CASE("as_vec2f")
  {
    const TilePos pos {-325, 834};
    const auto vec = pos.as_vec2f();

    REQUIRE(static_cast<float>(pos.row()) == vec.y);
    REQUIRE(static_cast<float>(pos.col()) == vec.x);
  }

  TEST_CASE("operator==")
  {
    const TilePos pos {45, 23};
    REQUIRE(pos == pos);

    const TilePos copy {pos};
    REQUIRE(pos == copy);
    REQUIRE(copy == pos);

    const TilePos diff {32, 93};
    REQUIRE_FALSE(pos == diff);
    REQUIRE_FALSE(diff == pos);
  }

  TEST_CASE("operator!=")
  {
    const TilePos pos {45, 23};
    REQUIRE_FALSE(pos != pos);

    const TilePos copy {pos};
    REQUIRE_FALSE(pos != copy);
    REQUIRE_FALSE(copy != pos);

    const TilePos other {839, 54};
    REQUIRE(pos != other);
    REQUIRE(other != pos);
  }
}

}  // namespace tactile::test
