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

#include "core/util/bits.hpp"

#include <limits>  // numeric_limits

#include <gtest/gtest.h>

namespace tactile::test {

constexpr auto max_u32 = std::numeric_limits<uint32>::max();

TEST(Bits, NthByteZeroU32)
{
  ASSERT_EQ(0, nth_byte(uint32 {0}, 0));
  ASSERT_EQ(0, nth_byte(uint32 {0}, 1));
  ASSERT_EQ(0, nth_byte(uint32 {0}, 2));
  ASSERT_EQ(0, nth_byte(uint32 {0}, 3));
}

TEST(Bits, NthByteMaxU32)
{
  ASSERT_EQ(0xFF, nth_byte(max_u32, 0));
  ASSERT_EQ(0xFF, nth_byte(max_u32, 1));
  ASSERT_EQ(0xFF, nth_byte(max_u32, 2));
  ASSERT_EQ(0xFF, nth_byte(max_u32, 3));
}

TEST(Bits, NthByteU8)
{
  const uint8 value = 0xCA;
  ASSERT_EQ(value, nth_byte(value, 0));
}

TEST(Bits, NthByteU16)
{
  const uint16 value = 0xBABE;
  ASSERT_EQ(0xBE, nth_byte(value, 0));
  ASSERT_EQ(0xBA, nth_byte(value, 1));
}

TEST(Bits, NthByteU32)
{
  const uint32 value = 0x1234'5678;
  ASSERT_EQ(0x78, nth_byte(value, 0));
  ASSERT_EQ(0x56, nth_byte(value, 1));
  ASSERT_EQ(0x34, nth_byte(value, 2));
  ASSERT_EQ(0x12, nth_byte(value, 3));
}

TEST(Bits, NthByteU64)
{
  const uint64 value = 0xDEAD'BEEF'CAFE'BABE;
  ASSERT_EQ(0xBE, nth_byte(value, 0));
  ASSERT_EQ(0xBA, nth_byte(value, 1));
  ASSERT_EQ(0xFE, nth_byte(value, 2));
  ASSERT_EQ(0xCA, nth_byte(value, 3));
  ASSERT_EQ(0xEF, nth_byte(value, 4));
  EXPECT_EQ(0xBE, nth_byte(value, 5));
  EXPECT_EQ(0xAD, nth_byte(value, 6));
  EXPECT_EQ(0xDE, nth_byte(value, 7));
}

}  // namespace tactile::test