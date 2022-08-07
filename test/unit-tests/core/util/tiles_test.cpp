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

#include "core/util/tiles.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

TEST(TileUtils, ToMatrixCoords)
{
  ASSERT_EQ(0, to_matrix_coords(0, 3).row);
  ASSERT_EQ(0, to_matrix_coords(0, 3).col);

  ASSERT_EQ(0, to_matrix_coords(1, 3).row);
  ASSERT_EQ(1, to_matrix_coords(1, 3).col);

  ASSERT_EQ(0, to_matrix_coords(2, 3).row);
  ASSERT_EQ(2, to_matrix_coords(2, 3).col);

  ASSERT_EQ(1, to_matrix_coords(3, 3).row);
  ASSERT_EQ(0, to_matrix_coords(3, 3).col);

  ASSERT_EQ(1, to_matrix_coords(4, 3).row);
  ASSERT_EQ(1, to_matrix_coords(4, 3).col);

  ASSERT_EQ(1, to_matrix_coords(5, 3).row);
  ASSERT_EQ(2, to_matrix_coords(5, 3).col);
}

TEST(TileUtils, MakeTileRow)
{
  const auto row = make_tile_row(10);
  ASSERT_EQ(10, row.capacity());
  ASSERT_EQ(10, row.size());
}

TEST(TileUtils, MakeTileMatrix)
{
  const auto matrix = make_tile_matrix(5, 7);
  ASSERT_EQ(5, matrix.capacity());
  ASSERT_EQ(5, matrix.size());
  ASSERT_EQ(7, matrix.at(0).capacity());
  ASSERT_EQ(7, matrix.at(0).size());
}

}  // namespace tactile::test
