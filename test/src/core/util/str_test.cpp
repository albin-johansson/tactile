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

#include "core/util/str.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

TEST(Str, Split)
{
  const auto tokens = split("foo,bar,,x", ',');

  ASSERT_EQ(4, tokens.size());
  ASSERT_EQ("foo", tokens.at(0));
  ASSERT_EQ("bar", tokens.at(1));
  ASSERT_EQ("", tokens.at(2));
  ASSERT_EQ("x", tokens.at(3));
}

TEST(Str, ParseI32)
{
  ASSERT_FALSE(parse_i32("ABC"));

  ASSERT_EQ(-42, parse_i32("-42"));
  ASSERT_EQ(123, parse_i32("123"));
  ASSERT_EQ(745, parse_i32("745"));

  ASSERT_FALSE(parse_i32("F1"));
  ASSERT_EQ(0xF1, parse_i32("F1", 16));
}

}  // namespace tactile::test
