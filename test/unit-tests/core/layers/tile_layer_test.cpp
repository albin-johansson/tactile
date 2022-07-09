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

#include "core/layers/tile_layer.hpp"

#include <memory>  // dynamic_pointer_cast

#include <gtest/gtest.h>

#include "core/common/functional.hpp"
#include "core/tile_pos.hpp"
#include "misc/panic.hpp"

namespace tactile::test {

TEST(TileLayer, Defaults)
{
  const TileLayer layer;

  ASSERT_EQ(5, layer.row_count());
  ASSERT_EQ(5, layer.column_count());

  invoke_mn(layer.row_count(), layer.column_count(), [&](usize row, usize col) {
    ASSERT_EQ(empty_tile, layer.tile_at(TilePos::from(row, col)));
  });
}

TEST(TileLayer, Flood)
{
  /* 0  0  0  0  0
     0  0  0  0  0
     0  0  0  0  0
     0  0  0  0  0
     0  0  0  0  0 */
  TileLayer layer {5, 5};

  /* 0  0  0  0  0
     0  0  0  1  1
     0  0  0  1  0
     0  1  1  1  0
     1  0  0  0  0 */
  layer.set_tile({1, 3}, 1);
  layer.set_tile({1, 4}, 1);
  layer.set_tile({2, 3}, 1);
  layer.set_tile({3, 1}, 1);
  layer.set_tile({3, 2}, 1);
  layer.set_tile({3, 3}, 1);
  layer.set_tile({4, 0}, 1);

  /* 0  0  0  0  0
     0  0  0  1  1
     0  0  0  1  2
     0  1  1  1  2
     1  2  2  2  2 */
  layer.flood({4, 1}, 2);

  ASSERT_EQ(0, layer.tile_at({0, 0}));
  ASSERT_EQ(0, layer.tile_at({0, 1}));
  ASSERT_EQ(0, layer.tile_at({0, 2}));
  ASSERT_EQ(0, layer.tile_at({0, 3}));
  ASSERT_EQ(0, layer.tile_at({0, 4}));

  ASSERT_EQ(0, layer.tile_at({1, 0}));
  ASSERT_EQ(0, layer.tile_at({1, 1}));
  ASSERT_EQ(0, layer.tile_at({1, 2}));
  ASSERT_EQ(1, layer.tile_at({1, 3}));
  ASSERT_EQ(1, layer.tile_at({1, 4}));

  ASSERT_EQ(0, layer.tile_at({2, 0}));
  ASSERT_EQ(0, layer.tile_at({2, 1}));
  ASSERT_EQ(0, layer.tile_at({2, 2}));
  ASSERT_EQ(1, layer.tile_at({2, 3}));
  ASSERT_EQ(2, layer.tile_at({2, 4}));

  ASSERT_EQ(0, layer.tile_at({3, 0}));
  ASSERT_EQ(1, layer.tile_at({3, 1}));
  ASSERT_EQ(1, layer.tile_at({3, 2}));
  ASSERT_EQ(1, layer.tile_at({3, 3}));
  ASSERT_EQ(2, layer.tile_at({3, 4}));

  ASSERT_EQ(1, layer.tile_at({4, 0}));
  ASSERT_EQ(2, layer.tile_at({4, 1}));
  ASSERT_EQ(2, layer.tile_at({4, 2}));
  ASSERT_EQ(2, layer.tile_at({4, 3}));
  ASSERT_EQ(2, layer.tile_at({4, 4}));

  /* 0  0  0  0  0
     0  0  0  3  3
     0  0  0  3  2
     0  3  3  3  2
     1  2  2  2  2 */
  layer.flood({3, 1}, 3);

  ASSERT_EQ(0, layer.tile_at({0, 0}));
  ASSERT_EQ(0, layer.tile_at({0, 1}));
  ASSERT_EQ(0, layer.tile_at({0, 2}));
  ASSERT_EQ(0, layer.tile_at({0, 3}));
  ASSERT_EQ(0, layer.tile_at({0, 4}));

  ASSERT_EQ(0, layer.tile_at({1, 0}));
  ASSERT_EQ(0, layer.tile_at({1, 1}));
  ASSERT_EQ(0, layer.tile_at({1, 2}));
  ASSERT_EQ(3, layer.tile_at({1, 3}));
  ASSERT_EQ(3, layer.tile_at({1, 4}));

  ASSERT_EQ(0, layer.tile_at({2, 0}));
  ASSERT_EQ(0, layer.tile_at({2, 1}));
  ASSERT_EQ(0, layer.tile_at({2, 2}));
  ASSERT_EQ(3, layer.tile_at({2, 3}));
  ASSERT_EQ(2, layer.tile_at({2, 4}));

  ASSERT_EQ(0, layer.tile_at({3, 0}));
  ASSERT_EQ(3, layer.tile_at({3, 1}));
  ASSERT_EQ(3, layer.tile_at({3, 2}));
  ASSERT_EQ(3, layer.tile_at({3, 3}));
  ASSERT_EQ(2, layer.tile_at({3, 4}));

  ASSERT_EQ(1, layer.tile_at({4, 0}));
  ASSERT_EQ(2, layer.tile_at({4, 1}));
  ASSERT_EQ(2, layer.tile_at({4, 2}));
  ASSERT_EQ(2, layer.tile_at({4, 3}));
  ASSERT_EQ(2, layer.tile_at({4, 4}));
}

TEST(TileLayer, SetTile)
{
  TileLayer layer;

  ASSERT_THROW(layer.set_tile({0, -1}, 42), TactileError);
  ASSERT_THROW(layer.set_tile({-1, 0}, 42), TactileError);

  ASSERT_THROW(layer.set_tile(TilePos::from(layer.row_count(), 0), 42), TactileError);
  ASSERT_THROW(layer.set_tile(TilePos::from(0, layer.column_count()), 42), TactileError);

  layer.set_tile({1, 2}, 42);
  ASSERT_EQ(42, layer.tile_at({1, 2}));

  const auto bottomRight = TilePos::from(layer.row_count() - 1, layer.column_count() - 1);
  layer.set_tile(bottomRight, 7);
  ASSERT_EQ(7, layer.tile_at(bottomRight));
}

TEST(TileLayer, TileAtWithInvalidPositions)
{
  TileLayer layer;

  ASSERT_THROW(layer.tile_at({-1, 0}), TactileError);
  ASSERT_THROW(layer.tile_at({0, -1}), TactileError);

  ASSERT_THROW(layer.tile_at(TilePos::from(layer.row_count(), 0)), TactileError);
  ASSERT_THROW(layer.tile_at(TilePos::from(0, layer.column_count())), TactileError);
}

TEST(TileLayer, AddRow)
{
  TileLayer layer {3, 4};

  ASSERT_EQ(3, layer.row_count());
  ASSERT_EQ(4, layer.column_count());

  layer.add_row();

  ASSERT_EQ(4, layer.row_count());
  ASSERT_EQ(4, layer.column_count());

  layer.add_row();

  ASSERT_EQ(5, layer.row_count());
  ASSERT_EQ(4, layer.column_count());
}

TEST(TileLayer, AddColumn)
{
  TileLayer layer {3, 4};

  ASSERT_EQ(3, layer.row_count());
  ASSERT_EQ(4, layer.column_count());

  layer.add_column();

  ASSERT_EQ(3, layer.row_count());
  ASSERT_EQ(5, layer.column_count());
}

TEST(TileLayer, RemoveRow)
{
  TileLayer layer {3, 4};

  ASSERT_EQ(3, layer.row_count());
  ASSERT_EQ(4, layer.column_count());

  layer.remove_row();

  ASSERT_EQ(2, layer.row_count());
  ASSERT_EQ(4, layer.column_count());
}

TEST(TileLayer, RemoveColumn)
{
  TileLayer layer {6, 8};

  ASSERT_EQ(6, layer.row_count());
  ASSERT_EQ(8, layer.column_count());

  layer.remove_column();

  ASSERT_EQ(6, layer.row_count());
  ASSERT_EQ(7, layer.column_count());
}

TEST(TileLayer, Resize)
{
  TileLayer layer {6, 5};

  ASSERT_EQ(6, layer.row_count());
  ASSERT_EQ(5, layer.column_count());
  ASSERT_FALSE(layer.is_valid({2, 8}));

  layer.resize(3, 9);

  ASSERT_EQ(3, layer.row_count());
  ASSERT_EQ(9, layer.column_count());
  ASSERT_TRUE(layer.is_valid({2, 8}));
}

TEST(TileLayer, Clone)
{
  TileLayer source {4, 3};

  source.set_tile({0, 0}, 7);
  source.set_tile({1, 2}, 8314);
  source.set_tile({2, 1}, 934);
  source.set_tile({3, 2}, 42);

  const auto copy = std::dynamic_pointer_cast<TileLayer>(source.clone());

  ASSERT_EQ(source.row_count(), copy->row_count());
  ASSERT_EQ(source.column_count(), copy->column_count());

  invoke_mn(source.row_count(), source.column_count(), [&](usize row, usize col) {
    const auto pos = TilePos::from(row, col);
    ASSERT_EQ(source.tile_at(pos), copy->tile_at(pos));
  });
}

}  // namespace tactile::test
