// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>   // integral, same_as
#include <limits>     // numeric_limits
#include <stdexcept>  // underflow_error, overflow_error
#include <utility>    // cmp_less, cmp_greater

namespace tactile {

/**
 * Performs a checked narrowing conversion of an integral value.
 *
 * \tparam To   The conversion target type.
 * \tparam From The conversion source type.
 *
 * \param from The value to narrow.
 *
 * \return
 * The narrowed value.
 *
 * \throws std::underflow_error If the value is too small for the target type.
 * \throws std::overflow_error  If the value is too large for the target type.
 */
template <std::integral To, std::integral From>
[[nodiscard]] constexpr auto narrow(const From from) -> To
{
  static_assert(sizeof(To) <= sizeof(From));

  if constexpr (std::same_as<To, From>) {
    return static_cast<To>(from);
  }
  else {
    constexpr auto to_min = std::numeric_limits<To>::min();
    constexpr auto to_max = std::numeric_limits<To>::max();

    if (std::cmp_less(from, to_min)) {
      throw std::underflow_error {"narrowed value would be too small"};
    }

    if (std::cmp_greater(from, to_max)) {
      throw std::overflow_error {"narrowed value would be too large"};
    }

    return static_cast<To>(from);
  }
}

}  // namespace tactile
