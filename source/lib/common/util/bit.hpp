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

#include <bit>       // byteswap, endian
#include <concepts>  // integral, invocable

#include "common/numeric.hpp"

namespace tactile {

template <std::integral Int, std::invocable<uint8> T>
void each_byte(const Int value, T&& callable)
{
  const auto* bytes = static_cast<const uint8*>(static_cast<const void*>(&value));
  for (usize idx = 0; idx < sizeof(Int); ++idx) {
    callable(bytes[idx]);
  }
}

[[nodiscard]] constexpr auto to_little_endian(const uint32 value) noexcept -> uint32
{
  return (std::endian::native == std::endian::little) ? value : std::byteswap(value);
}

[[nodiscard]] constexpr auto to_little_endian(const int32 value) noexcept -> int32
{
  return (std::endian::native == std::endian::little) ? value : std::byteswap(value);
}

}  // namespace tactile
