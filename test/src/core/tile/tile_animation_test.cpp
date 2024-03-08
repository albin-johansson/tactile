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

#include "core/tile/tile_animation.hpp"

#include <doctest/doctest.h>

#include "tactile/base/util/chrono.hpp"
#include "tactile/core/debug/exception.hpp"

using namespace std::chrono_literals;

namespace tactile::test {

TEST_SUITE("TileAnimation")
{
  TEST_CASE("Defaults")
  {
    const TileAnimation animation;
    REQUIRE(0u == animation.size());
    REQUIRE(0u == animation.capacity());
  }

  TEST_CASE("reserve_frames")
  {
    TileAnimation animation;

    animation.reserve_frames(3);
    REQUIRE(3u == animation.capacity());
    REQUIRE(0u == animation.size());
  }

  TEST_CASE("add_frame")
  {
    TileAnimation animation;

    animation.add_frame(42, 14ms);
    REQUIRE(1u == animation.size());

    const auto& frame = animation[0];
    REQUIRE(42 == frame.tile);
    REQUIRE(14ms == frame.duration);

    REQUIRE_THROWS_AS(animation[1], Exception);
  }

  TEST_CASE("insert_frame")
  {
    TileAnimation animation;

    REQUIRE(animation.insert_frame(0, 5, 100ms).has_value());
    REQUIRE(1u == animation.size());

    REQUIRE(animation.insert_frame(0, 20, 100ms).has_value());
    REQUIRE(2u == animation.size());

    REQUIRE(animation.insert_frame(animation.size(), 30, 100ms).has_value());
    REQUIRE(3u == animation.size());

    REQUIRE(!animation.insert_frame(animation.size() + 1, 40, 100ms).has_value());
    REQUIRE(3u == animation.size());
  }

  TEST_CASE("remove_frame")
  {
    TileAnimation animation;
    REQUIRE(!animation.remove_frame(0).has_value());

    animation.add_frame(12, 42ms);
    REQUIRE(1u == animation.size());

    REQUIRE(!animation.remove_frame(1).has_value());
    REQUIRE(1u == animation.size());

    REQUIRE(animation.remove_frame(0).has_value());
    REQUIRE(!animation.remove_frame(0).has_value());
    REQUIRE(0u == animation.size());
  }
}

}  // namespace tactile::test
