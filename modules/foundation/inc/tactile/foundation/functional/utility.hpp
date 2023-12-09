// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // integral, invocable

namespace tactile {

/**
 * \brief Invokes a function object the specified number of times.
 *
 * \tparam T an integral type.
 * \tparam U a callable type.
 *
 * \param times    the number of times the function object will be called.
 * \param callable the function object.
 */
template <std::integral T, std::invocable U>
constexpr void repeat(const T times, const U& callable) noexcept(noexcept(callable()))
{
  for (T index {}; index < times; ++index) {
    callable();
  }
}

}  // namespace tactile
