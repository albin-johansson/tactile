// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>     // integral, invocable, predicate
#include <type_traits>  // is_invocable_r_v

#include "tactile/core/debug/exception.hpp"

namespace tactile {

/// Invokes a function object N times.
///
/// \param n the amount of times the callable should be invoked.
/// \param callable a function object.
template <std::integral T, std::invocable U>
constexpr void invoke_n(const T n, U&& callable) noexcept(noexcept(callable()))
{
  for (T i = 0; i < n; ++i) {
    callable();
  }
}

/// Invokes a function object M*N times.
///
/// This function is a more functional approach to nested for-loops.
///
/// \param m the amount of times the outer loop is run.
/// \param n the amount of times the inner loop is run (for each outer loop iteration).
/// \param callable a function object.
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
  return predicate(value) ? value : throw Exception {"Value did not match requirements!"};
}

}  // namespace tactile
