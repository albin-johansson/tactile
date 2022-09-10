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

#include <bit>          // endian, byteswap
#include <concepts>     // integral
#include <type_traits>  // make_unsigned_t

#include "core/common/ints.hpp"

namespace tactile {

/// Returns the byte at a specific index in an integer (starting at the rightmost byte).
template <std::integral T>
[[nodiscard]] constexpr auto nth_byte(const T value, const uint n) noexcept -> uint8
{
  using UT = std::make_unsigned_t<T>;

  const auto offset = static_cast<UT>(n * 8u);
  const auto masked = static_cast<UT>(value) & (UT {0xFF} << offset);

  return static_cast<uint8>(masked >> offset);
}

template <std::integral T>
[[nodiscard]] constexpr auto as_little_endian(const T value) noexcept -> T
{
  return (std::endian::native == std::endian::little) ? value : std::byteswap(value);
}

}  // namespace tactile
