// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <bit>       // byteswap, endian
#include <concepts>  // integral, invocable

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/debug/assert.hpp"

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
 *    The N-th byte.
 */
template <typename T>
[[nodiscard]] auto nth_byte(const T& object, const usize n) noexcept -> uint8
{
  TACTILE_ASSERT_MSG(n < sizeof(T), "invalid byte index");
  const auto* bytes = static_cast<const uint8*>(static_cast<const void*>(&object));
  return bytes[n];
}

/**
 * Invokes a callback for each byte in an integer.
 *
 * \param value    The integer value to inspect.
 * \param callable The function object.
 */
template <std::integral IntType, std::invocable<uint8> CallableType>
void each_byte(const IntType value, const CallableType& callable)
{
  const auto* bytes = static_cast<const uint8*>(static_cast<const void*>(&value));
  for (usize idx = 0; idx < sizeof(IntType); ++idx) {
    callable(bytes[idx]);
  }
}

/**
 * Converts a native integer to a little endian integer.
 *
 * \param value The native integer value.
 *
 * \return
 *    A little endian integer value.
 */
template <std::integral IntType>
[[nodiscard]] constexpr auto to_little_endian(const IntType value) noexcept -> IntType
{
  return (std::endian::native == std::endian::little) ? value : std::byteswap(value);
}

}  // namespace tactile
