// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>    // reverse
#include <bit>          // bit_cast, endian, byteswap
#include <concepts>     // integral, invocable
#include <cstddef>      // byte
#include <type_traits>  // has_unique_object_representations_v
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
  using ByteArray = Array<std::byte, sizeof(IntType)>;

  const auto bytes = std::bit_cast<ByteArray>(value);
  std::reverse(bytes.begin(), bytes.end());

  return std::bit_cast<IntType>(bytes);
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
