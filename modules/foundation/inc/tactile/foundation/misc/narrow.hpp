// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // integral, signed_integral, same_as
#include <limits>    // numeric_limits
#include <utility>   // cmp_less_equal, cmp_greater_equal

#include "tactile/foundation/debug/assert.hpp"
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
[[nodiscard]] constexpr auto narrow_checked(const From value) -> To
{
  if constexpr (std::same_as<From, To>) {
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
 * Performs an unchecked narrowing conversion of an integer.
 *
 * \tparam To   The conversion target type.
 * \tparam From Any integral type.
 *
 * \param value The original value.
 *
 * \return
 *    The narrowed input value.
 */
template <std::integral To, std::integral From>
  requires(sizeof(To) <= sizeof(From))
[[nodiscard]] constexpr auto narrow_cast(const From value) noexcept -> To
{
  TACTILE_ASSERT(std::cmp_greater_equal(value, std::numeric_limits<To>::min()));
  TACTILE_ASSERT(std::cmp_less_equal(value, std::numeric_limits<To>::max()));
  return static_cast<To>(value);
}

}  // namespace tactile
