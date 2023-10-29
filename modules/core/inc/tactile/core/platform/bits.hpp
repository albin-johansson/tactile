// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>    // reverse
#include <bit>          // bit_cast, endian, byteswap
#include <concepts>     // integral, invocable
#include <cstring>      // memcpy
#include <type_traits>  // has_unique_object_representations_v
#include <type_traits>  // is_trivially_copyable_v, is_trivially_constructible_v
#include <version>      // __cpp_lib_byteswap

#include "tactile/core/container/array.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Invokes a callback for each byte in an integer.
 *
 * \param value    the integer value to inspect.
 * \param callable the function object.
 */
template <std::integral IntType, std::invocable<uint8> CallableType>
void each_byte(const IntType value, const CallableType& callable)
{
  const auto* bytes = static_cast<const uint8*>(static_cast<const void*>(&value));
  for (usize idx = 0; idx < sizeof(IntType); ++idx) {
    callable(bytes[idx]);
  }
}

template <typename To, typename From>
  requires(sizeof(To) == sizeof(From) &&          //
           std::is_trivially_copyable_v<From> &&  //
           std::is_trivially_copyable_v<To> &&    //
           std::is_trivially_constructible_v<To>)
[[nodiscard]] auto interpret_as(const From& src) noexcept -> To
{
#if __cpp_lib_bit_cast >= 201806L
  return std::bit_cast<To>(src);
#else
  To dst;
  std::memcpy(&dst, &src, sizeof dst);
  return dst;
#endif
}

/**
 * \brief Reverses the bytes in an integer value.
 *
 * \tparam IntType an integral type.
 *
 * \param value an integer value.
 *
 * \return a byteswapped integer.
 */
template <std::integral IntType>
[[nodiscard]] constexpr auto reverse_bytes(const IntType value) noexcept -> IntType
{
#if __cpp_lib_byteswap >= 202110L
  return std::byteswap(value);
#else
  using ByteArray = Array<uint8, sizeof(IntType)>;

  auto bytes = interpret_as<ByteArray>(value);
  std::reverse(bytes.begin(), bytes.end());

  return interpret_as<IntType>(bytes);
#endif
}

/**
 * \brief Converts a native integer to a little endian integer.
 *
 * \param value the native integer value.
 *
 * \return a little endian integer value.
 */
template <std::integral IntType>
[[nodiscard]] constexpr auto to_little_endian(const IntType value) noexcept -> IntType
{
  return (std::endian::native == std::endian::little) ? value : reverse_bytes(value);
}

}  // namespace tactile
