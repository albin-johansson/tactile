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

#include "core/layer/tile_layer.hpp"

#include <memory>  // dynamic_pointer_cast

#include <doctest/doctest.h>

#include "common/util/functional.hpp"
#include "core/debug/panic.hpp"
#include "core/tile/tile_pos.hpp"

namespace tactile::test {

TEST_SUITE("TileLayer")
{
  TEST_CASE("Defaults")
  {
    const TileLayer layer;

    REQUIRE(5u == layer.row_count());
    REQUIRE(5u == layer.column_count());

    invoke_mn(layer.row_count(), layer.column_count(), [&](usize row, usize col) {
      REQUIRE(empty_tile == layer.tile_at(TilePos::from(row, col)));
    });
  }

  TEST_CASE("flood")
  {
    // 0  0  0  0  0
    // 0  0  0  0  0
    // 0  0  0  0  0
    // 0  0  0  0  0
    // 0  0  0  0  0
    TileLayer layer {5, 5};

    // 0  0  0  0  0
    // 0  0  0  1  1
    // 0  0  0  1  0
    // 0  1  1  1  0
    // 1  0  0  0  0
    layer.set_tile({1, 3}, 1);
    layer.set_tile({1, 4}, 1);
    layer.set_tile({2, 3}, 1);
    layer.set_tile({3, 1}, 1);
    layer.set_tile({3, 2}, 1);
    layer.set_tile({3, 3}, 1);
    layer.set_tile({4, 0}, 1);

    // 0  0  0  0  0
    // 0  0  0  1  1
    // 0  0  0  1  2
    // 0  1  1  1  2
    // 1  2  2  2  2
    layer.flood({4, 1}, 2);

    REQUIRE(0 == layer.tile_at({0, 0}));
    REQUIRE(0 == layer.tile_at({0, 1}));
    REQUIRE(0 == layer.tile_at({0, 2}));
    REQUIRE(0 == layer.tile_at({0, 3}));
    REQUIRE(0 == layer.tile_at({0, 4}));

    REQUIRE(0 == layer.tile_at({1, 0}));
    REQUIRE(0 == layer.tile_at({1, 1}));
    REQUIRE(0 == layer.tile_at({1, 2}));
    REQUIRE(1 == layer.tile_at({1, 3}));
    REQUIRE(1 == layer.tile_at({1, 4}));

    REQUIRE(0 == layer.tile_at({2, 0}));
    REQUIRE(0 == layer.tile_at({2, 1}));
    REQUIRE(0 == layer.tile_at({2, 2}));
    REQUIRE(1 == layer.tile_at({2, 3}));
    REQUIRE(2 == layer.tile_at({2, 4}));

    REQUIRE(0 == layer.tile_at({3, 0}));
    REQUIRE(1 == layer.tile_at({3, 1}));
    REQUIRE(1 == layer.tile_at({3, 2}));
    REQUIRE(1 == layer.tile_at({3, 3}));
    REQUIRE(2 == layer.tile_at({3, 4}));

    REQUIRE(1 == layer.tile_at({4, 0}));
    REQUIRE(2 == layer.tile_at({4, 1}));
    REQUIRE(2 == layer.tile_at({4, 2}));
    REQUIRE(2 == layer.tile_at({4, 3}));
    REQUIRE(2 == layer.tile_at({4, 4}));

    // 0  0  0  0  0
    // 0  0  0  3  3
    // 0  0  0  3  2
    // 0  3  3  3  2
    // 1  2  2  2  2
    layer.flood({3, 1}, 3);

    REQUIRE(0 == layer.tile_at({0, 0}));
    REQUIRE(0 == layer.tile_at({0, 1}));
    REQUIRE(0 == layer.tile_at({0, 2}));
    REQUIRE(0 == layer.tile_at({0, 3}));
    REQUIRE(0 == layer.tile_at({0, 4}));

    REQUIRE(0 == layer.tile_at({1, 0}));
    REQUIRE(0 == layer.tile_at({1, 1}));
    REQUIRE(0 == layer.tile_at({1, 2}));
    REQUIRE(3 == layer.tile_at({1, 3}));
    REQUIRE(3 == layer.tile_at({1, 4}));

    REQUIRE(0 == layer.tile_at({2, 0}));
    REQUIRE(0 == layer.tile_at({2, 1}));
    REQUIRE(0 == layer.tile_at({2, 2}));
    REQUIRE(3 == layer.tile_at({2, 3}));
    REQUIRE(2 == layer.tile_at({2, 4}));

    REQUIRE(0 == layer.tile_at({3, 0}));
    REQUIRE(3 == layer.tile_at({3, 1}));
    REQUIRE(3 == layer.tile_at({3, 2}));
    REQUIRE(3 == layer.tile_at({3, 3}));
    REQUIRE(2 == layer.tile_at({3, 4}));

    REQUIRE(1 == layer.tile_at({4, 0}));
    REQUIRE(2 == layer.tile_at({4, 1}));
    REQUIRE(2 == layer.tile_at({4, 2}));
    REQUIRE(2 == layer.tile_at({4, 3}));
    REQUIRE(2 == layer.tile_at({4, 4}));
  }

  TEST_CASE("set_tile")
  {
    TileLayer layer;

    REQUIRE_THROWS_AS(layer.set_tile({0, -1}, 42), TactileError);
    REQUIRE_THROWS_AS(layer.set_tile({-1, 0}, 42), TactileError);

    REQUIRE_THROWS_AS(layer.set_tile(TilePos::from(layer.row_count(), 0), 42),
                      TactileError);
    REQUIRE_THROWS_AS(layer.set_tile(TilePos::from(0, layer.column_count()), 42),
                      TactileError);

    layer.set_tile({1, 2}, 42);
    REQUIRE(42 == layer.tile_at({1, 2}));

    const auto bottom_right =
        TilePos::from(layer.row_count() - 1, layer.column_count() - 1);
    layer.set_tile(bottom_right, 7);
    REQUIRE(7 == layer.tile_at(bottom_right));
  }

  TEST_CASE("tile_at (with invalid positions)")
  {
    const TileLayer layer;

    REQUIRE_THROWS_AS(layer.tile_at({-1, 0}), TactileError);
    REQUIRE_THROWS_AS(layer.tile_at({0, -1}), TactileError);

    REQUIRE_THROWS_AS(layer.tile_at(TilePos::from(layer.row_count(), 0)), TactileError);
    REQUIRE_THROWS_AS(layer.tile_at(TilePos::from(0, layer.column_count())),
                      TactileError);
  }

  TEST_CASE("add_row")
  {
    TileLayer layer {3, 4};

    REQUIRE(3u == layer.row_count());
    REQUIRE(4u == layer.column_count());

    layer.add_row();

    REQUIRE(4u == layer.row_count());
    REQUIRE(4u == layer.column_count());

    layer.add_row();

    REQUIRE(5u == layer.row_count());
    REQUIRE(4u == layer.column_count());
  }

  TEST_CASE("add_column")
  {
    TileLayer layer {3, 4};

    REQUIRE(3u == layer.row_count());
    REQUIRE(4u == layer.column_count());

    layer.add_column();

    REQUIRE(3u == layer.row_count());
    REQUIRE(5u == layer.column_count());
  }

  TEST_CASE("remove_row")
  {
    TileLayer layer {3, 4};

    REQUIRE(3u == layer.row_count());
    REQUIRE(4u == layer.column_count());

    layer.remove_row();

    REQUIRE(2u == layer.row_count());
    REQUIRE(4u == layer.column_count());
  }

  TEST_CASE("remove_column")
  {
    TileLayer layer {6, 8};

    REQUIRE(6u == layer.row_count());
    REQUIRE(8u == layer.column_count());

    layer.remove_column();

    REQUIRE(6u == layer.row_count());
    REQUIRE(7u == layer.column_count());
  }

  TEST_CASE("resize")
  {
    TileLayer layer {6, 5};

    REQUIRE(6u == layer.row_count());
    REQUIRE(5u == layer.column_count());
    REQUIRE(!layer.is_valid({2, 8}));

    layer.resize(3, 9);

    REQUIRE(3u == layer.row_count());
    REQUIRE(9u == layer.column_count());
    REQUIRE(layer.is_valid({2, 8}));
  }

  TEST_CASE("clone")
  {
    TileLayer source {4, 3};

    source.set_tile({0, 0}, 7);
    source.set_tile({1, 2}, 8314);
    source.set_tile({2, 1}, 934);
    source.set_tile({3, 2}, 42);

    const auto copy = std::dynamic_pointer_cast<TileLayer>(source.clone());

    REQUIRE(source.row_count() == copy->row_count());
    REQUIRE(source.column_count() == copy->column_count());

    invoke_mn(source.row_count(), source.column_count(), [&](usize row, usize col) {
      const auto pos = TilePos::from(row, col);
      REQUIRE(source.tile_at(pos) == copy->tile_at(pos));
    });
  }
}

}  // namespace tactile::test
