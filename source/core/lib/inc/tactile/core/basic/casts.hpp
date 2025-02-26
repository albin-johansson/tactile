// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>   // integral
#include <limits>     // numeric_limits
#include <stdexcept>  // range_error
#include <utility>    // cmp_less, cmp_greater

namespace tactile {

/// Performs a saturated integral narrowing conversion.
///
/// The original value is clamped to be between the minimum and maximum `To` values.
template <std::integral To, std::integral From>
[[nodiscard]] constexpr auto saturate_cast(const From& from) noexcept -> To
{
  constexpr auto to_min = std::numeric_limits<To>::min();
  constexpr auto to_max = std::numeric_limits<To>::max();

  if (std::cmp_less(from, to_min)) {
    return to_min;
  }

  if (std::cmp_greater(from, to_max)) {
    return to_max;
  }

  return static_cast<To>(from);
}

/// Performs a checked integral narrowing conversion.
template <std::integral To, std::integral From>
[[nodiscard]] constexpr auto narrow_cast(const From& from) -> To
{
  constexpr auto to_min = std::numeric_limits<To>::min();
  constexpr auto to_max = std::numeric_limits<To>::max();

  if (std::cmp_less(from, to_min) || std::cmp_greater(from, to_max)) {
    throw std::range_error {"narrowing conversion failed"};
  }

  return static_cast<To>(from);
}

}  // namespace tactile
