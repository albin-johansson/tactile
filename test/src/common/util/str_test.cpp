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

#include "common/util/str.hpp"

#include <doctest/doctest.h>

namespace tactile::test {

TEST_SUITE("Str")
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
}

}  // namespace tactile::test
