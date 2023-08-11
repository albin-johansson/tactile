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

#include "tactile/core/common/string_conv.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(StringConv, StrToI32)
{
  EXPECT_FALSE(str_to_i32("").has_value());
  EXPECT_FALSE(str_to_i32("-").has_value());
  EXPECT_FALSE(str_to_i32("F").has_value());
  EXPECT_FALSE(str_to_i32("foo").has_value());

  EXPECT_EQ(str_to_i32("0"), 0);
  EXPECT_EQ(str_to_i32("42"), 42);
  EXPECT_EQ(str_to_i32("-83"), -83);
  EXPECT_EQ(str_to_i32("01"), 1);
  EXPECT_EQ(str_to_i32("111", 2), 0b111);
  EXPECT_EQ(str_to_i32("E9C", 16), 0xE9C);
}

TEST(StringConv, StrToU32)
{
  EXPECT_FALSE(str_to_u32("").has_value());
  EXPECT_FALSE(str_to_u32("+").has_value());
  EXPECT_FALSE(str_to_u32("A").has_value());
  EXPECT_FALSE(str_to_u32(" ").has_value());

  EXPECT_EQ(str_to_u32("0"), 0u);
  EXPECT_EQ(str_to_u32("123"), 123u);
  EXPECT_EQ(str_to_u32("007"), 7u);
  EXPECT_EQ(str_to_u32("101", 2), 0b101u);
  EXPECT_EQ(str_to_u32("B5", 16), 0xB5u);
}

TEST(StringConv, StrToF32)
{
  EXPECT_FALSE(str_to_f32("").has_value());
  EXPECT_FALSE(str_to_f32("*").has_value());
  EXPECT_FALSE(str_to_f32("X").has_value());
  EXPECT_FALSE(str_to_f32(" ").has_value());

  EXPECT_EQ(str_to_f32("0"), 0.0f);
  EXPECT_EQ(str_to_f32("4.5"), 4.5f);
  EXPECT_EQ(str_to_f32("-9"), -9.0f);
  EXPECT_EQ(str_to_f32("10"), 10.0f);
}
