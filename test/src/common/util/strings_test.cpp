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

#include "common/util/strings.hpp"

#include <doctest/doctest.h>

using namespace tactile;

TEST_SUITE("String utilities")
{
  TEST_CASE("split")
  {
    const auto tokens = split("foo,bar,,x", ',');

    REQUIRE(4u == tokens.size());
    REQUIRE("foo" == tokens.at(0));
    REQUIRE("bar" == tokens.at(1));
    REQUIRE("" == tokens.at(2));
    REQUIRE("x" == tokens.at(3));
  }

  TEST_CASE("parse_i32")
  {
    REQUIRE(!parse_i32("ABC"));

    REQUIRE(-42 == parse_i32("-42"));
    REQUIRE(123 == parse_i32("123"));
    REQUIRE(745 == parse_i32("745"));

    REQUIRE(!parse_i32("F1"));
    REQUIRE(0xF1 == parse_i32("F1", 16));
  }

  TEST_CASE("parse_f32")
  {
    REQUIRE(!parse_f32("ABC"));

    REQUIRE(-42.0f == parse_f32("-42"));
    REQUIRE(123.0f == parse_f32("123"));
    REQUIRE(1.0f == parse_f32("1.0"));
    REQUIRE(12.5f == parse_f32("12.5"));

    SUBCASE("Ensure function respects string view range")
    {
      const String str {"1234"};
      const StringView full_view {str};
      REQUIRE(12.0f == parse_f32(full_view.substr(0, 2)));
      REQUIRE(34.0f == parse_f32(full_view.substr(2, 2)));
    }
  }
}
