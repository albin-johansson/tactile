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

#include "core/platform/bit_utils.hpp"

#include <bit>  // bit_cast

#include <doctest/doctest.h>

#include "core/containers/array.hpp"

using namespace tactile;

namespace {

template <std::integral T>
[[nodiscard]] auto _nth_byte(const T value, const usize n) -> uint8
{
  const auto* bytes = static_cast<const uint8*>(static_cast<const void*>(&value));
  return bytes[n];
}

}  // namespace

TEST_SUITE("BitUtils")
{
  TEST_CASE("reverse_bytes")
  {
    using ByteArray = Array<uint8, sizeof(uint32)>;

    const uint32 original = 0xDEADBEEF;
    const auto swapped = reverse_bytes(original);

    const auto original_bytes = std::bit_cast<ByteArray>(original);
    const auto swapped_bytes = std::bit_cast<ByteArray>(swapped);

    CHECK(original_bytes[0] == swapped_bytes[3]);
    CHECK(original_bytes[1] == swapped_bytes[2]);
    CHECK(original_bytes[2] == swapped_bytes[1]);
    CHECK(original_bytes[3] == swapped_bytes[0]);
  }

  TEST_CASE("to_little_endian[uint32]")
  {
    const uint32 original = 0xFF'EE'22'11u;
    const auto little = to_little_endian(original);

    CHECK(_nth_byte(little, 0) == 0x11);
    CHECK(_nth_byte(little, 1) == 0x22);
    CHECK(_nth_byte(little, 2) == 0xEE);
    CHECK(_nth_byte(little, 3) == 0xFF);
  }

  TEST_CASE("to_little_endian[int32]")
  {
    const int32 original = 0x11'22'33'44;
    const auto little = to_little_endian(original);

    CHECK(_nth_byte(little, 0) == 0x44);
    CHECK(_nth_byte(little, 1) == 0x33);
    CHECK(_nth_byte(little, 2) == 0x22);
    CHECK(_nth_byte(little, 3) == 0x11);
  }
}