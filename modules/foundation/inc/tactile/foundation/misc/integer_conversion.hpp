// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>     // integral, unsigned_integral, signed_integral
#include <limits>       // numeric_limits
#include <type_traits>  // make_unsigned_t, make_signed_t, is_same_v
#include <utility>      // cmp_less_equal, cmp_greater_equal

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Performs a checked narrowing conversion of an integer.
 *
 * \tparam To   The conversion target type.
 * \tparam From Any integral type.
 *
 * \param value The original value.
 *
 * \return
 *    The narrowed input value.
 *
 * \throws Exception if the conversion would lose information.
 */
template <std::integral To, std::integral From>
  requires(sizeof(To) <= sizeof(From))
[[nodiscard]] constexpr auto narrow(const From value) -> To
{
  if constexpr (std::is_same_v<From, To>) {
    return value;
  }
  else {
    constexpr auto kToMin = std::numeric_limits<To>::min();
    constexpr auto kToMax = std::numeric_limits<To>::max();

    // We only check the lower limit when narrowing signed integers.
    // This is because any unsigned "From" integer will be greater
    // than the "To" integer type minimum value, regardless of the
    // signedness of "To".
    if constexpr (std::signed_integral<From>) {
      if (std::cmp_greater_equal(value, kToMin) && std::cmp_less_equal(value, kToMax)) {
        return static_cast<To>(value);
      }
    }
    else {
      if (std::cmp_less_equal(value, kToMax)) {
        return static_cast<To>(value);
      }
    }

    throw Exception {"lossy integer narrowing conversion"};
  }
}

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

}  // namespace tactile
