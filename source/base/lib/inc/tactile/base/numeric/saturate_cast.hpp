// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // integral
#include <limits>    // numeric_limits
#include <utility>   // cmp_less, cmp_greater

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Converts an integer value to a value of another integer type.
 *
 * \tparam To   The new type.
 * \tparam From The old type.
 *
 * \param from The value to convert.
 *
 * \return
 * The passed value if it's representable using the target type. Otherwise, the
 * largest or smallest value of the target type, whichever is closest to the
 * passed value.
 *
 * \see \c std::saturate_cast (C++26)
 */
template <std::integral To, std::integral From>
[[nodiscard]] constexpr auto saturate_cast(const From from) noexcept -> To
{
  if (const auto to_min = std::numeric_limits<To>::min(); std::cmp_less(from, to_min)) {
    return to_min;
  }

  if (const auto to_max = std::numeric_limits<To>::max(); std::cmp_greater(from, to_max)) {
    return to_max;
  }

  return static_cast<To>(from);
}

}  // namespace tactile
