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

#include "core/util/numeric.hpp"

#include <limits>  // numeric_limits

#include <gtest/gtest.h>

namespace tactile::test {
namespace {

constexpr auto u32_max = std::numeric_limits<uint32>::max();

}  // namespace

TEST(Numeric, Udiff)
{
  ASSERT_EQ(1u, udiff(u32_max, u32_max - 1));
  ASSERT_EQ(1u, udiff(u32_max - 1, u32_max));

  ASSERT_EQ(u32_max, udiff(0u, u32_max));

  ASSERT_EQ(46u, udiff(28u, 74u));
}

TEST(Numeric, AsLittleEndianU32)
{
  const auto value = as_little_endian(0xA1B2C3D4);
  const auto* bytes = static_cast<const uint8*>(static_cast<const void*>(&value));

  ASSERT_EQ(0xD4, bytes[0]);
  ASSERT_EQ(0xC3, bytes[1]);
  ASSERT_EQ(0xB2, bytes[2]);
  ASSERT_EQ(0xA1, bytes[3]);
}

TEST(Numeric, AsLittleEndianI32)
{
  const auto source = static_cast<int32>(0xA1B2C3D4);

  const auto value = as_little_endian(source);
  const auto* bytes = static_cast<const uint8*>(static_cast<const void*>(&value));

  ASSERT_EQ(0xD4, bytes[0]);
  ASSERT_EQ(0xC3, bytes[1]);
  ASSERT_EQ(0xB2, bytes[2]);
  ASSERT_EQ(0xA1, bytes[3]);
}

}  // namespace tactile::test