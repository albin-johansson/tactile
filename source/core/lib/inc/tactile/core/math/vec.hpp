// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/basic/primitives.hpp"

namespace tactile {

template <typename T>
struct Vec2D final
{
  using value_type = T;

  value_type x;
  value_type y;

  [[nodiscard]]
  constexpr auto operator<=>(const Vec2D&) const = default;
};

using Float2 = Vec2D<f32>;
using Int2 = Vec2D<i32>;

template <typename T>
[[nodiscard]] constexpr auto operator+(const Vec2D<T>& lhs, const Vec2D<T>& rhs) noexcept
    -> Vec2D<T>
{
  return {lhs.x + rhs.x, lhs.y + rhs.y};
}

template <typename T>
[[nodiscard]] constexpr auto operator-(const Vec2D<T>& lhs, const Vec2D<T>& rhs) noexcept
    -> Vec2D<T>
{
  return {lhs.x - rhs.x, lhs.y - rhs.y};
}

template <typename T>
[[nodiscard]] constexpr auto operator*(const Vec2D<T>& vec, const T factor) noexcept
    -> Vec2D<T>
{
  return {vec.x * factor, vec.y * factor};
}

template <typename T>
[[nodiscard]] constexpr auto operator*(const T factor, const Vec2D<T>& vec) noexcept
    -> Vec2D<T>
{
  return vec * factor;
}

template <typename T>
constexpr auto operator+=(Vec2D<T>& lhs, const Vec2D<T>& rhs) noexcept -> Vec2D<T>&
{
  lhs = lhs + rhs;
  return lhs;
}

template <typename T>
constexpr auto operator-=(Vec2D<T>& lhs, const Vec2D<T>& rhs) noexcept -> Vec2D<T>&
{
  lhs = lhs - rhs;
  return lhs;
}

template <typename T>
constexpr auto operator*=(Vec2D<T>& vec, const T factor) noexcept -> Vec2D<T>&
{
  vec = vec * factor;
  return vec;
}

}  // namespace tactile
