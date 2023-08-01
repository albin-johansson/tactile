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

#pragma once

#include <algorithm>    // reverse
#include <bit>          // bit_cast, endian
#include <concepts>     // integral, invocable
#include <cstddef>      // byte
#include <type_traits>  // has_unique_object_representations_v

#include "common/type/array.hpp"
#include "core/prelude.hpp"

namespace tactile {

template <std::integral Int, std::invocable<uint8> T>
void each_byte(const Int value, T&& callable)
{
  const auto* bytes = static_cast<const uint8*>(static_cast<const void*>(&value));
  for (usize idx = 0; idx < sizeof(Int); ++idx) {
    callable(bytes[idx]);
  }
}

template <std::integral T>
[[nodiscard]] constexpr auto reverse_bytes(const T value) noexcept -> T
{
  using ByteArray = Array<std::byte, sizeof(T)>;

  static_assert(std::has_unique_object_representations_v<T>,
                "Must not have padding bits");

  auto bytes = std::bit_cast<ByteArray>(value);
  std::ranges::reverse(bytes);

  return std::bit_cast<T>(bytes);
}

[[nodiscard]] constexpr auto to_little_endian(const uint32 value) noexcept -> uint32
{
  return (std::endian::native == std::endian::little) ? value : reverse_bytes(value);
}

[[nodiscard]] constexpr auto to_little_endian(const int32 value) noexcept -> int32
{
  return (std::endian::native == std::endian::little) ? value : reverse_bytes(value);
}

}  // namespace tactile
