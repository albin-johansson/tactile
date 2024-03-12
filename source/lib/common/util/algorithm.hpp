// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>  // find_if
#include <concepts>   // unsigned_integral, predicate
#include <utility>    // forward

#include "tactile/base/container/vector.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

/// Returns the difference between two unsigned integers
template <std::unsigned_integral T>
[[nodiscard]] constexpr auto udiff(const T a, const T b) noexcept -> T
{
  return (a < b) ? (b - a) : (a - b);
}

/// Returns the first element in a vector that satisfies the given predicate.
template <typename T, std::predicate<const T&> P>
[[nodiscard]] auto first_in(Vector<T>& vec, P&& pred) -> T&
{
  const auto iter = std::find_if(vec.begin(), vec.end(), std::forward<P>(pred));
  if (iter != vec.end()) {
    return *iter;
  }
  else {
    throw Exception {"No element matched predicate!"};
  }
}

/// Returns the first element in a vector that satisfies the given predicate.
template <typename T, std::predicate<const T&> P>
[[nodiscard]] auto first_in(const Vector<T>& vec, P&& pred) -> const T&
{
  const auto iter = std::find_if(vec.begin(), vec.end(), std::forward<P>(pred));
  if (iter != vec.end()) {
    return *iter;
  }
  else {
    throw Exception {"No element matched predicate!"};
  }
}

}  // namespace tactile
