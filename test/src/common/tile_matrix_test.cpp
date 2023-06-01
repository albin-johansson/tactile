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

#include "common/tile_matrix.hpp"

#include <doctest/doctest.h>

using namespace tactile;

TEST_SUITE("TileMatrix")
{
  TEST_CASE("to_matrix_coords")
  {
    REQUIRE(0 == to_matrix_coords(0, 3).row);
    REQUIRE(0 == to_matrix_coords(0, 3).col);

    REQUIRE(0 == to_matrix_coords(1, 3).row);
    REQUIRE(1 == to_matrix_coords(1, 3).col);

    REQUIRE(0 == to_matrix_coords(2, 3).row);
    REQUIRE(2 == to_matrix_coords(2, 3).col);

    REQUIRE(1 == to_matrix_coords(3, 3).row);
    REQUIRE(0 == to_matrix_coords(3, 3).col);

    REQUIRE(1 == to_matrix_coords(4, 3).row);
    REQUIRE(1 == to_matrix_coords(4, 3).col);

    REQUIRE(1 == to_matrix_coords(5, 3).row);
    REQUIRE(2 == to_matrix_coords(5, 3).col);
  }

  TEST_CASE("make_tile_row")
  {
    const auto row = make_tile_row(10);
    REQUIRE(10u == row.capacity());
    REQUIRE(10u == row.size());
  }

  TEST_CASE("make_tile_matrix")
  {
    const auto matrix = make_tile_matrix(TileExtent {5, 7});
    REQUIRE(5u == matrix.capacity());
    REQUIRE(5u == matrix.size());
    REQUIRE(7u == matrix.at(0).capacity());
    REQUIRE(7u == matrix.at(0).size());
  }
}
