// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include "tactile/core/concepts.hpp"
#include "tactile/core/primitives.hpp"

namespace tactile {

/// Represents a two-dimensional vector.
///
/// \tparam T The value type.
template <Number T>
struct Vec2 final
{
  using value_type = T;

  /// The X coordinate.
  value_type x {};

  /// The Y coordinate.
  value_type y {};

  constexpr auto operator==(const Vec2&) const noexcept -> bool = default;
};

/// Performs element-wise addition of two vectors.
///
/// \tparam    T
///            The value type.
/// \param[in] lhs
///            The left-hand side vector.
/// \param[in] rhs
///            The right-hand side vector.
/// \returns   A vector constructed from element-wise addition.
template <Number T>
constexpr auto operator+(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept -> Vec2<T>
{
  return Vec2<T> {lhs.x + rhs.x, lhs.y + rhs.y};
}

/// Performs element-wise subtraction of two vectors.
///
/// \tparam    T
///            The value type.
/// \param[in] lhs
///            The left-hand side vector.
/// \param[in] rhs
///            The right-hand side vector.
/// \returns   A vector constructed from element-wise subtraction.
template <Number T>
constexpr auto operator-(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept -> Vec2<T>
{
  return Vec2<T> {lhs.x - rhs.x, lhs.y - rhs.y};
}

/// Performs element-wise multiplication of two vectors.
///
/// \tparam    T
///            The value type.
/// \param[in] lhs
///            The left-hand side vector.
/// \param[in] rhs
///            The right-hand side vector.
/// \returns   A vector constructed from element-wise multiplication.
template <Number T>
constexpr auto operator*(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept -> Vec2<T>
{
  return Vec2<T> {lhs.x * rhs.x, lhs.y * rhs.y};
}

/// Scales a vector by a given factor.
///
/// \tparam    T
///            The value type.
/// \param[in] vec
///            The vector to scale.
/// \param[in] factor
///            The scale factor to apply.
/// \returns   A scaled version of the original vector.
template <Number T>
constexpr auto operator*(const Vec2<T>& vec, const T factor) noexcept -> Vec2<T>
{
  return Vec2<T> {vec.x * factor, vec.y * factor};
}

/// Alias for a two-dimensional vector with `float32` values.
using Vec2F = Vec2<float32>;

/// Alias for a two-dimensional vector with `float64` values.
using Vec2D = Vec2<float64>;

}  // namespace tactile
