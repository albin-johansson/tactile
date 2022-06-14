/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <concepts>     // integral, invocable, predicate
#include <type_traits>  // is_invocable_r_v

#include "misc/panic.hpp"

namespace tactile {

/// A concept similar to std::invocable, but with explicit return type
template <typename T, typename Ret, typename... Args>
concept Callable = std::is_invocable_r_v<Ret, T, Args...>;

/**
 * Invokes a function object N times.
 *
 * \param n the amount of times the callable should be invoked.
 * \param callable a function object.
 */
template <std::integral T, std::invocable U>
constexpr void invoke_n(const T n, U&& callable) noexcept(noexcept(callable()))
{
  for (T i = 0; i < n; ++i) {
    callable();
  }
}

/**
 * Invokes a function object M*N times.
 *
 * This function is a more functional approach to nested for-loops.
 *
 * \param m the amount of times the outer loop is run.
 * \param n the amount of times the inner loop is run (for each outer loop iteration).
 * \param callable a function object.
 */
template <std::integral T, std::invocable<T, T> U>
constexpr void invoke_mn(const T m,
                         const T n,
                         U&& callable) noexcept(noexcept(callable(m, n)))
{
  for (T i = 0; i < m; ++i) {
    for (T j = 0; j < n; ++j) {
      callable(i, j);
    }
  }
}

/// Ensures that a value matches a predicate and returns it, throws otherwise.
template <typename T, std::predicate<const T&> U>
constexpr auto require_that(T value, U&& predicate) -> T
{
  return predicate(value) ? value
                          : throw TactileError{"Value did not match requirements!"};
}

}  // namespace tactile
