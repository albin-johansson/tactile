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

#include "core/tile/tile.hpp"

#include <utility>  // move

#include <doctest/doctest.h>

using namespace std::chrono_literals;

namespace tactile::test {

TEST_SUITE("Tile")
{
  TEST_CASE("Defaults")
  {
    const Tile tile {7};
    REQUIRE(7 == tile.get_index());
    REQUIRE(0u == tile.object_count());
    REQUIRE(Int4 {} == tile.get_source());
    REQUIRE(!tile.is_animated());
  }

  TEST_CASE("add_object")
  {
    Tile tile {42};

    auto object = std::make_shared<Object>();
    tile.add_object(object);

    REQUIRE(1u == tile.object_count());
  }

  TEST_CASE("clear_animation")
  {
    Tile tile {42};

    tile.set_animation(TileAnimation {});
    REQUIRE(tile.is_animated());

    tile.clear_animation();
    REQUIRE(!tile.is_animated());
  }

  TEST_CASE("set_animation")
  {
    Tile tile {42};

    {
      TileAnimation animation;
      animation.add_frame(7, 42ms);
      animation.add_frame(8, 37ms);
      tile.set_animation(std::move(animation));
    }

    REQUIRE(tile.is_animated());
    const auto& animation = tile.get_animation();

    REQUIRE(2u == animation.size());

    REQUIRE(7 == animation[0].tile);
    REQUIRE(42ms == animation[0].duration);

    REQUIRE(8 == animation[1].tile);
    REQUIRE(37ms == animation[1].duration);
  }

  TEST_CASE("set_source")
  {
    Tile tile {100};

    const Int4 source {27, 82, 45, 32};
    tile.set_source(source);

    REQUIRE(source == tile.get_source());
  }
}

}  // namespace tactile::test
