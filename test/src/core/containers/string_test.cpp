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

#include "core/containers/string.hpp"

#include <doctest/doctest.h>

using namespace tactile;

TEST_SUITE("StringUtils")
{
  TEST_CASE("split")
  {
    const auto tokens = split("foo,bar,,x", ',');

    REQUIRE(tokens.size() == 4u);
    REQUIRE(tokens.at(0) == "foo");
    REQUIRE(tokens.at(1) == "bar");
    REQUIRE(tokens.at(2) == "");
    REQUIRE(tokens.at(3) == "x");
  }

  TEST_CASE("parse_i32")
  {
    REQUIRE(!parse_i32("ABC"));

    REQUIRE(parse_i32("-42") == -42);
    REQUIRE(parse_i32("123") == 123);
    REQUIRE(parse_i32("745") == 745);

    REQUIRE(!parse_i32("F1"));
    REQUIRE(parse_i32("F1", 16) == 0xF1);
  }

  TEST_CASE("parse_f32")
  {
    REQUIRE(!parse_f32("ABC"));

    REQUIRE(parse_f32("-42") == -42.0f);
    REQUIRE(parse_f32("123") == 123.0f);
    REQUIRE(parse_f32("1.0") == 1.0f);
    REQUIRE(parse_f32("12.5") == 12.5f);

    SUBCASE("Ensure function respects string view range")
    {
      const String str {"1234"};
      const StringView full_view {str};
      REQUIRE(parse_f32(full_view.substr(0, 2)) == 12.0f);
      REQUIRE(parse_f32(full_view.substr(2, 2)) == 34.0f);
    }
  }
}
