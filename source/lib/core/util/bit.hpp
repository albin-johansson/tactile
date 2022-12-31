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

#include <algorithm>    // reverse
#include <bit>          // bit_cast, endian
#include <concepts>     // integral, invocable
#include <cstring>      // memcpy
#include <type_traits>  // has_unique_object_representations_v, is_trivially_copyable_v, is_trivially_constructible_v
#include <version>      // __cpp_lib_bit_cast

#include "common/type/array.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

template <typename To, typename From>
  requires(sizeof(To) == sizeof(From) &&          //
           std::is_trivially_copyable_v<From> &&  //
           std::is_trivially_copyable_v<To> &&    //
           std::is_trivially_constructible_v<To>)
[[nodiscard]] auto bitcast(const From& src) noexcept -> To
{
#if __cpp_lib_bit_cast >= 201806L
  return std::bit_cast<To>(src);
#else
  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
#endif  // __cpp_lib_bit_cast >= 201806L
}

template <std::integral T>
[[nodiscard]] constexpr auto byteswap(T value) noexcept -> T
{
  // Based on example implementation: https://en.cppreference.com/w/cpp/numeric/byteswap
  static_assert(std::has_unique_object_representations_v<T>,
                "T may not have padding bits");

  using Bytes = Array<uint8, sizeof(value)>;

  auto bytes = bitcast<Bytes>(value);
  std::reverse(std::begin(bytes), std::end(bytes));

  return bitcast<T>(bytes);
}

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
  return (std::endian::native == std::endian::little) ? value : byteswap(value);
}

[[nodiscard]] constexpr auto to_little_endian(const int32 value) noexcept -> int32
{
  return (std::endian::native == std::endian::little) ? value : byteswap(value);
}

}  // namespace tactile
