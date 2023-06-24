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

#include "common/util/buffers.hpp"

#include <doctest/doctest.h>

#include "common/type/array.hpp"

using namespace tactile;

TEST_SUITE("BufferUtils")
{
  TEST_CASE("zero_buffer")
  {
    Array<char, 5> buffer = {'0', '1', '2', '3', '4'};
    zero_buffer(buffer);

    CHECK(buffer[0] == 0);
    CHECK(buffer[1] == 0);
    CHECK(buffer[2] == 0);
    CHECK(buffer[3] == 0);
    CHECK(buffer[4] == 0);
  }

  TEST_CASE("copy_string_into_buffer")
  {
    Array<char, 10> buffer = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    copy_string_into_buffer(buffer, "abcdef");

    CHECK(buffer.at(0) == 'a');
    CHECK(buffer.at(1) == 'b');
    CHECK(buffer.at(2) == 'c');
    CHECK(buffer.at(3) == 'd');
    CHECK(buffer.at(4) == 'e');
    CHECK(buffer.at(5) == 'f');

    // The rest of the buffer should have been zeroed
   CHECK(buffer.at(6) == 0);
   CHECK(buffer.at(7) == 0);
   CHECK(buffer.at(8) == 0);
   CHECK(buffer.at(9) == 0);
  }

  TEST_CASE("copy_string_into_buffer (with buffer that is too small)")
  {
    Array<char, 4> buffer = {'0', '1', '2', '3'};
    copy_string_into_buffer(buffer, "abcdef");

    CHECK(buffer.at(0) == 'a');
    CHECK(buffer.at(1) == 'b');
    CHECK(buffer.at(2) == 'c');
    CHECK(buffer.at(3) == 'd');
  }

  TEST_CASE("create_string_from_buffer")
  {
    const Array<char, 7> buffer = {'a', 'b', 'c', 'd', 'e', 'f', '\0'};
    const auto str = create_string_from_buffer(buffer);

    CHECK(str == "abcdef");
  }

  TEST_CASE("create_string_view_from_buffer")
  {
    const Array<char, 7> buffer = {'a', 'b', 'c', 'd', 'e', 'f', '\0'};
    const auto str = create_string_view_from_buffer(buffer);

    CHECK(str.size() == 6u);
    CHECK(str == "abcdef");
  }
}
