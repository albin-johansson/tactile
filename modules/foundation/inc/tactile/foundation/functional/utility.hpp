// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // integral, invocable

namespace tactile {

/**
 * Invokes a function object the specified number of times.
 *
 * \tparam T An integral type.
 * \tparam U A callable type.
 *
 * \param times    The number of times the function object will be called.
 * \param callable The function object.
 */
template <std::integral T, std::invocable U>
constexpr void repeat(const T times, const U& callable) noexcept(noexcept(callable()))
{
  for (T index {}; index < times; ++index) {
    callable();
  }
}

}  // namespace tactile
