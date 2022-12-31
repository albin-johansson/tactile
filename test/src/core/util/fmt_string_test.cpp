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

#include <fmt/std.h>
#include <gtest/gtest.h>

#include "common/util/fmt.hpp"

namespace tactile::test {

TEST(FmtString, NoFormatting)
{
  const FmtString str {"foobar"};

  ASSERT_STREQ("foobar", str.data());
  ASSERT_EQ("foobar", str.view());

  ASSERT_EQ(6u, str.size());
  ASSERT_EQ(128u, str.capacity());
}

TEST(FmtString, SimpleFormatting)
{
  const FmtString str {"Hello, {}!", "World"};
  ASSERT_EQ("Hello, World!", str.view());
}

TEST(FmtString, AdvancedFormatting)
{
  const std::filesystem::path path = "foo/bar.txt";
  const FmtString str {"The answer is {}, here's a path: {}", 42, path};
  ASSERT_EQ("The answer is 42, here's a path: \"foo/bar.txt\"", str.view());
}

TEST(FmtString, SpareCapacity)
{
  const FmtString<16> str {"123"};

  ASSERT_EQ("123", str.view());
  ASSERT_STREQ("123", str.data());

  ASSERT_EQ(3u, str.size());
  ASSERT_EQ(16u, str.capacity());
}

TEST(FmtString, ExactCapacity)
{
  const FmtString<5> str {"12345"};

  ASSERT_EQ("12345", str.view());
  ASSERT_STREQ("12345", str.data());

  ASSERT_EQ(5u, str.size());
  ASSERT_EQ(5u, str.capacity());
}

TEST(FmtString, NotEnoughCapacity)
{
  const FmtString<4> str {"12345"};

  ASSERT_EQ("1234", str.view());
  ASSERT_STREQ("1234", str.data());

  ASSERT_EQ(4u, str.size());
  ASSERT_EQ(4u, str.capacity());
}

TEST(FmtString, FormatArgumentCapacityOverflow)
{
  const FmtString<6> str {"1234{}", "567"};

  ASSERT_EQ("123456", str.view());
  ASSERT_STREQ("123456", str.data());

  ASSERT_EQ(6u, str.size());
  ASSERT_EQ(6u, str.capacity());
}

}  // namespace tactile::test
