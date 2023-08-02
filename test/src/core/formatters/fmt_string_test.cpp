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

#include "core/formatters/fmt_string.hpp"

#include <filesystem>  // path

#include <doctest/doctest.h>
#include <fmt/std.h>

using namespace std::string_view_literals;

using namespace tactile;

TEST_SUITE("FmtString")
{
  TEST_CASE("No formatting")
  {
    const FmtString str {"foobar"};

    CHECK(str.data() == "foobar"sv);
    CHECK(str.view() == "foobar");

    CHECK(str.size() == 6u);
    CHECK(str.capacity() == 128u);
  }

  TEST_CASE("Simple formatting")
  {
    const FmtString str {"Hello, {}!", "World"};
    CHECK(str.view() == "Hello, World!");
  }

  TEST_CASE("Advanced formatting")
  {
    const std::filesystem::path path = "foo/bar.txt";
    const FmtString str {"The answer is {}, here's a path: {}", 42, path};
    CHECK(str.view() == "The answer is 42, here's a path: \"foo/bar.txt\"");
  }

  TEST_CASE("Spare capacity")
  {
    const FmtString<16> str {"123"};

    CHECK(str.view() == "123");
    CHECK(str.data() == "123"sv);

    CHECK(str.size() == 3u);
    CHECK(str.capacity() == 16u);
  }

  TEST_CASE("Exact capacity")
  {
    const FmtString<5> str {"12345"};

    CHECK(str.view() == "12345");
    CHECK(str.data() == "12345"sv);

    CHECK(str.size() == 5u);
    CHECK(str.capacity() == 5u);
  }

  TEST_CASE("Not enough capacity")
  {
    const FmtString<4> str {"12345"};

    CHECK(str.view() == "1234");
    CHECK(str.data() == "1234"sv);

    CHECK(str.size() == 4u);
    CHECK(str.capacity() == 4u);
  }

  TEST_CASE("Format argument capacity overflow")
  {
    const FmtString<6> str {"1234{}", "567"};

    CHECK(str.view() == "123456");
    CHECK(str.data() == "123456"sv);

    CHECK(str.size() == 6u);
    CHECK(str.capacity() == 6u);
  }
}
