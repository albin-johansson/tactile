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

#include "core/util/string.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

TEST(Strings, IntegerFromString)
{
  ASSERT_FALSE(from_string<int>("ABC"));

  ASSERT_EQ(-42, from_string<int>("-42"));
  ASSERT_EQ(123, from_string<int16>("123"));
  ASSERT_EQ(745, from_string<int64>("745"));

  ASSERT_FALSE(from_string<int>("F1"));
  ASSERT_EQ(0xF1, from_string<int>("F1", 16));

  ASSERT_EQ(4, from_string<int>("4298", 1u, 10));
  ASSERT_EQ(0x12, from_string<int>("12AC", 2u, 16));

  // Robustness against invalid length
  ASSERT_EQ(1234, from_string<int>("1234"));
  ASSERT_EQ(1234, from_string<int>("1234", 4u, 10));
  ASSERT_EQ(1234, from_string<int>("1234", 5u, 10));
  ASSERT_EQ(1234, from_string<int>("1234", 6u, 10));
}

TEST(Strings, FloatFromString)
{
  ASSERT_FALSE(from_string<float>("foo"));

  ASSERT_EQ(12.3f, from_string<float>("12.3"));
  ASSERT_EQ(42.0f, from_string<float>("42"));

  ASSERT_EQ(78.5, from_string<double>("78.5"));
  ASSERT_EQ(98.0, from_string<double>("98"));
}

}  // namespace tactile::test
