// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <bit>       // endian
#include <concepts>  // integral, invocable

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
  return (std::endian::native == std::endian::little) ? value : std::byteswap(value);
}

}  // namespace tactile
