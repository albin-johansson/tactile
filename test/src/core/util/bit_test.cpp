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

#include "core/util/bit.hpp"

#include <gtest/gtest.h>

#include "core/type/array.hpp"

namespace tactile::test {
namespace {

template <std::integral T>
[[nodiscard]] auto nth_byte(const T value, const usize n) -> uint8
{
  const auto* bytes = static_cast<const uint8*>(static_cast<const void*>(&value));
  return bytes[n];
}

}  // namespace

TEST(Bit, Byteswap)
{
  const uint32 original = 0xDEADBEEF;
  const auto swapped = byteswap(original);

  using ByteArray = Array<uint8, sizeof(uint32)>;
  const auto original_bytes = bitcast<ByteArray>(original);
  const auto swapped_bytes = bitcast<ByteArray>(swapped);

  ASSERT_EQ(original_bytes.at(0), swapped_bytes.at(3));
  ASSERT_EQ(original_bytes.at(1), swapped_bytes.at(2));
  ASSERT_EQ(original_bytes.at(2), swapped_bytes.at(1));
  ASSERT_EQ(original_bytes.at(3), swapped_bytes.at(0));
}

TEST(Bit, ToLittleEndianUint32)
{
  const uint32 original = 0xFF'EE'22'11;
  const auto little = to_little_endian(original);

  ASSERT_EQ(0x11, nth_byte(little, 0));
  ASSERT_EQ(0x22, nth_byte(little, 1));
  ASSERT_EQ(0xEE, nth_byte(little, 2));
  ASSERT_EQ(0xFF, nth_byte(little, 3));
}

TEST(Bit, ToLittleEndianInt32)
{
  const int32 original = 0x11'22'33'44;
  const auto little = to_little_endian(original);

  ASSERT_EQ(0x44, nth_byte(little, 0));
  ASSERT_EQ(0x33, nth_byte(little, 1));
  ASSERT_EQ(0x22, nth_byte(little, 2));
  ASSERT_EQ(0x11, nth_byte(little, 3));
}

}  // namespace tactile::test