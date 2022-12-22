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

#include "core/util/buffers.hpp"

#include <gtest/gtest.h>

#include "core/type/array.hpp"

namespace tactile::test {

TEST(BufferUtils, ZeroBuffer)
{
  Array<char, 5> buffer = {'0', '1', '2', '3', '4'};
  zero_buffer(buffer);

  ASSERT_EQ(0, buffer[0]);
  ASSERT_EQ(0, buffer[1]);
  ASSERT_EQ(0, buffer[2]);
  ASSERT_EQ(0, buffer[3]);
  ASSERT_EQ(0, buffer[4]);
}

TEST(BufferUtils, CopyStringIntoBuffer)
{
  Array<char, 10> buffer = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  copy_string_into_buffer(buffer, "abcdef");

  ASSERT_EQ('a', buffer.at(0));
  ASSERT_EQ('b', buffer.at(1));
  ASSERT_EQ('c', buffer.at(2));
  ASSERT_EQ('d', buffer.at(3));
  ASSERT_EQ('e', buffer.at(4));
  ASSERT_EQ('f', buffer.at(5));

  // The rest of the buffer should have been zeroed
  ASSERT_EQ(0, buffer.at(6));
  ASSERT_EQ(0, buffer.at(7));
  ASSERT_EQ(0, buffer.at(8));
  ASSERT_EQ(0, buffer.at(9));
}

TEST(BufferUtils, CopyStringIntoBufferThatIsTooSmall)
{
  Array<char, 4> buffer = {'0', '1', '2', '3'};
  copy_string_into_buffer(buffer, "abcdef");

  ASSERT_EQ('a', buffer.at(0));
  ASSERT_EQ('b', buffer.at(1));
  ASSERT_EQ('c', buffer.at(2));
  ASSERT_EQ('d', buffer.at(3));
}

TEST(BufferUtils, CreateStringFromBuffer)
{
  const Array<char, 7> buffer = {'a', 'b', 'c', 'd', 'e', 'f', '\0'};
  const auto str = create_string_from_buffer(buffer);

  ASSERT_EQ("abcdef", str);
}

TEST(BufferUtils, CreateStringViewFromBuffer)
{
  const Array<char, 7> buffer = {'a', 'b', 'c', 'd', 'e', 'f', '\0'};
  const auto str = create_string_view_from_buffer(buffer);

  ASSERT_EQ(6u, str.size());
  ASSERT_EQ("abcdef", str);
}

}  // namespace tactile::test
