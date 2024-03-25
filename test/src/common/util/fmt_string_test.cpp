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

#include <filesystem>  // path

#include <doctest/doctest.h>
#include <fmt/std.h>

#include "common/util/fmt.hpp"

using namespace std::string_view_literals;

namespace tactile::test {

TEST_SUITE("FmtString")
{
  TEST_CASE("No formatting")
  {
    const FmtString str {"foobar"};

    REQUIRE("foobar"sv == str.data());
    REQUIRE("foobar" == str.view());

    REQUIRE(6u == str.size());
    REQUIRE(128u == str.capacity());
  }

  TEST_CASE("Simple formatting")
  {
    const FmtString str {"Hello, {}!", "World"};
    REQUIRE("Hello, World!" == str.view());
  }

  TEST_CASE("Advanced formatting")
  {
    const std::filesystem::path path = "foo/bar.txt";
    const FmtString str {"The answer is {}, here's a path: {}", 42, path};
    REQUIRE("The answer is 42, here's a path: foo/bar.txt" == str.view());
  }

  TEST_CASE("Spare capacity")
  {
    const FmtString<16> str {"123"};

    REQUIRE("123" == str.view());
    REQUIRE("123"sv == str.data());

    REQUIRE(3u == str.size());
    REQUIRE(16u == str.capacity());
  }

  TEST_CASE("Exact capacity")
  {
    const FmtString<5> str {"12345"};

    REQUIRE("12345" == str.view());
    REQUIRE("12345"sv == str.data());

    REQUIRE(5u == str.size());
    REQUIRE(5u == str.capacity());
  }

  TEST_CASE("Not enough capacity")
  {
    const FmtString<4> str {"12345"};

    REQUIRE("1234" == str.view());
    REQUIRE("1234"sv == str.data());

    REQUIRE(4u == str.size());
    REQUIRE(4u == str.capacity());
  }

  TEST_CASE("Format argument capacity overflow")
  {
    const FmtString<6> str {"1234{}", "567"};

    REQUIRE("123456" == str.view());
    REQUIRE("123456"sv == str.data());

    REQUIRE(6u == str.size());
    REQUIRE(6u == str.capacity());
  }
}

}  // namespace tactile::test
