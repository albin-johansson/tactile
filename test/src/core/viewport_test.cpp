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

#include "core/viewport.hpp"

#include <doctest/doctest.h>

#include "common/util/functional.hpp"

namespace tactile::test {

// TODO zoom_in, zoom_out

TEST_SUITE("Viewport")
{
  TEST_CASE("Defaults")
  {
    const Viewport viewport;
    REQUIRE(!viewport.limits().has_value());

    REQUIRE(0 == viewport.get_offset().x);
    REQUIRE(0 == viewport.get_offset().y);

    REQUIRE(32 == viewport.tile_size().x);
    REQUIRE(32 == viewport.tile_size().y);
  }

  TEST_CASE("reset_limits")
  {
    Viewport viewport;

    viewport.set_limits({.min_offset = {23, 12}, .max_offset = {83, 123}});
    REQUIRE(viewport.limits().has_value());

    viewport.reset_limits();
    REQUIRE(!viewport.limits().has_value());
  }

  TEST_CASE("offset")
  {
    Viewport viewport;

    viewport.offset({42, -12});
    REQUIRE(42 == viewport.get_offset().x);
    REQUIRE(-12 == viewport.get_offset().y);
  }

  TEST_CASE("pan_left")
  {
    Viewport viewport;

    viewport.pan_left();
    REQUIRE(viewport.tile_size().x == viewport.get_offset().x);
    REQUIRE(0 == viewport.get_offset().y);
  }

  TEST_CASE("pan_right")
  {
    Viewport viewport;

    viewport.pan_right();
    REQUIRE(-viewport.tile_size().x == viewport.get_offset().x);
    REQUIRE(0 == viewport.get_offset().y);
  }

  TEST_CASE("pan_up")
  {
    Viewport viewport;

    viewport.pan_up();
    REQUIRE(0 == viewport.get_offset().x);
    REQUIRE(viewport.tile_size().y == viewport.get_offset().y);
  }

  TEST_CASE("pan_down")
  {
    Viewport viewport;

    viewport.pan_down();
    REQUIRE(0 == viewport.get_offset().x);
    REQUIRE(-viewport.tile_size().y == viewport.get_offset().y);
  }

  TEST_CASE("panning in opposite directions should cancel out")
  {
    Viewport viewport;

    viewport.pan_left();
    viewport.pan_right();
    REQUIRE(0 == viewport.get_offset().x);
    REQUIRE(0 == viewport.get_offset().y);

    viewport.pan_up();
    viewport.pan_down();
    REQUIRE(0 == viewport.get_offset().x);
    REQUIRE(0 == viewport.get_offset().y);
  }

  TEST_CASE("set_tile_size")
  {
    Viewport viewport;

    viewport.set_tile_size({93, 24});
    REQUIRE(93 == viewport.tile_size().x);
    REQUIRE(24 == viewport.tile_size().y);
  }

  TEST_CASE("set_limits")
  {
    const float min_x = -32;
    const float min_y = -13;
    const float max_x = 483;
    const float max_y = 923;

    Viewport viewport;
    viewport.set_limits({.min_offset = {min_x, min_y}, .max_offset = {max_x, max_y}});

    viewport.offset({min_x - 1, 0});
    REQUIRE(min_x == viewport.get_offset().x);
    REQUIRE(0 == viewport.get_offset().y);

    viewport.offset({0, min_y - 1});
    REQUIRE(min_x == viewport.get_offset().x);
    REQUIRE(min_y == viewport.get_offset().y);

    viewport.offset({max_x + 100, 0});
    REQUIRE(max_x == viewport.get_offset().x);
    REQUIRE(min_y == viewport.get_offset().y);

    viewport.offset({0, max_y + 100});
    REQUIRE(max_x == viewport.get_offset().x);
    REQUIRE(max_y == viewport.get_offset().y);
  }
}

}  // namespace tactile::test
