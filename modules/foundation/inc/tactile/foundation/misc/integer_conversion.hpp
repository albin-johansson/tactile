// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>     // unsigned_integral, signed_integral
#include <limits>       // numeric_limits
#include <type_traits>  // make_unsigned_t, make_signed_t
#include <utility>      // cmp_less_equal

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/misc/narrow.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Converts an unsigned integer to a signed integer of the same width.
 *
 * \tparam T Any unsigned integer type.
 *
 * \param value The value to convert.
 *
 * \return
 *    A signed integer.
 *
 * \throws Exception if the conversion would overflow.
 */
template <std::unsigned_integral T>
[[nodiscard]] constexpr auto as_signed(const T value) -> std::make_signed_t<T>
{
  using signed_type = std::make_signed_t<T>;

  if (std::cmp_less_equal(value, std::numeric_limits<signed_type>::max())) [[likely]] {
    return static_cast<signed_type>(value);
  }

  throw Exception {"conversion to signed integer would overflow"};
}

/**
 * Converts a signed integer to an unsigned integer of the same width.
 *
 * \tparam T Any signed integer type.
 *
 * \param value The value to convert.
 *
 * \return
 *    An unsigned integer.
 *
 * \throws Exception if the conversion would underflow.
 */
template <std::signed_integral T>
[[nodiscard]] constexpr auto as_unsigned(const T value) -> std::make_unsigned_t<T>
{
  if (value >= T {0}) [[likely]] {
    return static_cast<std::make_unsigned_t<T>>(value);
  }

  throw Exception {"conversion to unsigned integer would underflow"};
}

template <typename T>
[[nodiscard]] constexpr auto ssizeof() noexcept -> ssize
{
  return narrow_cast<ssize>(sizeof(T));
}

template <typename T>
[[nodiscard]] constexpr auto ssizeof(const T& obj) noexcept -> ssize
{
  return narrow_cast<ssize>(sizeof(obj));
}

}  // namespace tactile
