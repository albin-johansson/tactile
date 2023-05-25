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

#include <doctest/doctest.h>

#include "common/type/array.hpp"
#include "common/util/bits.hpp"

namespace tactile::test {
namespace {

template <std::integral T>
[[nodiscard]] auto _nth_byte(const T value, const usize n) -> uint8
{
  const auto* bytes = static_cast<const uint8*>(static_cast<const void*>(&value));
  return bytes[n];
}

}  // namespace

TEST_SUITE("Bit utilities")
{
  TEST_CASE("to_little_endian[uint32]")
  {
    const uint32 original = 0xFF'EE'22'11u;
    const auto little = to_little_endian(original);

    REQUIRE(0x11 == _nth_byte(little, 0));
    REQUIRE(0x22 == _nth_byte(little, 1));
    REQUIRE(0xEE == _nth_byte(little, 2));
    REQUIRE(0xFF == _nth_byte(little, 3));
  }

  TEST_CASE("to_little_endian[int32]")
  {
    const int32 original = 0x11'22'33'44;
    const auto little = to_little_endian(original);

    REQUIRE(0x44 == _nth_byte(little, 0));
    REQUIRE(0x33 == _nth_byte(little, 1));
    REQUIRE(0x22 == _nth_byte(little, 2));
    REQUIRE(0x11 == _nth_byte(little, 3));
  }
}

}  // namespace tactile::test