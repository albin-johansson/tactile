// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>    // reverse
#include <bit>          // bit_cast, endian
#include <concepts>     // integral, invocable
#include <cstring>      // memcpy
#include <type_traits>  // has_unique_object_representations_v, is_trivially_copyable_v, is_trivially_constructible_v
#include <version>      // __cpp_lib_bit_cast

#include "tactile/base/container/array.hpp"
#include "tactile/base/int.hpp"

#if __cpp_lib_bit_cast >= 201806L
  #define TACTILE_HAS_STD_BITCAST 1
#else
  #define TACTILE_HAS_STD_BITCAST 0
#endif  // __cpp_lib_bit_cast >= 201806L

namespace tactile {

template <typename To, typename From>
concept BitCastable = sizeof(To) == sizeof(From) &&          //
                      std::is_trivially_copyable_v<From> &&  //
                      std::is_trivially_copyable_v<To> &&    //
                      std::is_trivially_constructible_v<To>;

template <typename To, typename From>
  requires BitCastable<To, From>
[[nodiscard]] auto bitcast(const From& src) noexcept -> To
{
#if TACTILE_HAS_STD_BITCAST
  return std::bit_cast<To>(src);
#else
  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
#endif  // TACTILE_HAS_STD_BITCAST
}

template <std::integral T>
[[nodiscard]] auto byteswap(const T value) noexcept -> T
{
  // Based on example implementation: https://en.cppreference.com/w/cpp/numeric/byteswap
  static_assert(std::has_unique_object_representations_v<T>,
                "T may not have padding bits");

  using ByteArray = Array<uint8, sizeof(value)>;

  auto bytes = bitcast<ByteArray>(value);
  std::reverse(bytes.begin(), bytes.end());

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

[[nodiscard]] inline auto to_little_endian(const uint32 value) noexcept -> uint32
{
  return (std::endian::native == std::endian::little) ? value : byteswap(value);
}

[[nodiscard]] inline auto to_little_endian(const int32 value) noexcept -> int32
{
  return (std::endian::native == std::endian::little) ? value : byteswap(value);
}

}  // namespace tactile
