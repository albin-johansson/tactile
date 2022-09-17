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

#pragma once

#include <concepts>  // integral, unsigned_integral, invocable

#include "core/common/vocabulary.hpp"

namespace tactile {

/// Returns the difference between two unsigned integers
template <std::unsigned_integral T>
[[nodiscard]] constexpr auto udiff(const T a, const T b) noexcept -> T
{
  return (a < b) ? (b - a) : (a - b);
}

[[nodiscard]] auto as_little_endian(int32 value) noexcept -> int32;
[[nodiscard]] auto as_little_endian(uint32 value) noexcept -> uint32;

template <std::integral Int, std::invocable<uint8> T>
void each_byte(const Int value, T&& callable)
{
  const auto* bytes = static_cast<const uint8*>(static_cast<const void*>(&value));
  for (usize idx = 0; idx < sizeof(Int); ++idx) {
    callable(bytes[idx]);
  }
}

}  // namespace tactile
