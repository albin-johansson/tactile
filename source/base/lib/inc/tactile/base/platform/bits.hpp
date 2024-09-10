// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <bit>       // byteswap, endian
#include <cassert>   // assert
#include <concepts>  // integral, invocable
#include <cstddef>   // size_t
#include <cstdint>   // uint8_t

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Reads the N-th byte of an object.
 *
 * \tparam T Any type.
 *
 * \param object The source object.
 * \param n      The index of the byte to read.
 *
 * \return
 * The N-th byte.
 */
template <typename T>
[[nodiscard]] auto nth_byte(const T& object, const std::size_t n) noexcept -> std::uint8_t
{
  assert(n < sizeof(T));
  const auto* bytes = reinterpret_cast<const std::uint8_t*>(&object);
  return bytes[n];
}

/**
 * Invokes a callback for each byte in an integer.
 *
 * \param value    The integer value to inspect.
 * \param callable The function object.
 */
template <std::integral IntType, std::invocable<std::uint8_t> CallableType>
void each_byte(const IntType value, const CallableType& callable)
{
  const auto* bytes = reinterpret_cast<const std::uint8_t*>(&value);
  for (std::size_t idx = 0; idx < sizeof(IntType); ++idx) {
    callable(bytes[idx]);
  }
}

/**
 * Converts a native integer to a little endian integer.
 *
 * \param value The native integer value.
 *
 * \return
 * A little endian integer value.
 */
template <std::integral IntType>
[[nodiscard]] constexpr auto to_little_endian(const IntType value) noexcept -> IntType
{
  return (std::endian::native == std::endian::little) ? value : std::byteswap(value);
}

}  // namespace tactile
