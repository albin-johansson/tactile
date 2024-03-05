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

#include "common/util/buffers.hpp"

#include <doctest/doctest.h>

#include "tactile/base/container/array.hpp"

namespace tactile::test {

TEST_SUITE("Buffers")
{
  TEST_CASE("zero_buffer")
  {
    Array<char, 5> buffer = {'0', '1', '2', '3', '4'};
    zero_buffer(buffer);

    REQUIRE(0 == buffer[0]);
    REQUIRE(0 == buffer[1]);
    REQUIRE(0 == buffer[2]);
    REQUIRE(0 == buffer[3]);
    REQUIRE(0 == buffer[4]);
  }

  TEST_CASE("copy_string_into_buffer")
  {
    Array<char, 10> buffer = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    copy_string_into_buffer(buffer, "abcdef");

    REQUIRE('a' == buffer.at(0));
    REQUIRE('b' == buffer.at(1));
    REQUIRE('c' == buffer.at(2));
    REQUIRE('d' == buffer.at(3));
    REQUIRE('e' == buffer.at(4));
    REQUIRE('f' == buffer.at(5));

    // The rest of the buffer should have been zeroed
    REQUIRE(0 == buffer.at(6));
    REQUIRE(0 == buffer.at(7));
    REQUIRE(0 == buffer.at(8));
    REQUIRE(0 == buffer.at(9));
  }

  TEST_CASE("copy_string_into_buffer (with buffer that is too small)")
  {
    Array<char, 4> buffer = {'0', '1', '2', '3'};
    copy_string_into_buffer(buffer, "abcdef");

    REQUIRE('a' == buffer.at(0));
    REQUIRE('b' == buffer.at(1));
    REQUIRE('c' == buffer.at(2));
    REQUIRE('d' == buffer.at(3));
  }

  TEST_CASE("create_string_from_buffer")
  {
    const Array<char, 7> buffer = {'a', 'b', 'c', 'd', 'e', 'f', '\0'};
    const auto str = create_string_from_buffer(buffer);

    REQUIRE("abcdef" == str);
  }

  TEST_CASE("create_string_view_from_buffer")
  {
    const Array<char, 7> buffer = {'a', 'b', 'c', 'd', 'e', 'f', '\0'};
    const auto str = create_string_view_from_buffer(buffer);

    REQUIRE(6u == str.size());
    REQUIRE("abcdef" == str);
  }
}

}  // namespace tactile::test
