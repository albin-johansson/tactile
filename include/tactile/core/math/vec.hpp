// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include "tactile/core/concepts.hpp"
#include "tactile/core/container/array.hpp"
#include "tactile/core/container/span.hpp"
#include "tactile/core/primitives.hpp"

namespace tactile {

template <Number T>
class Vec2 final
{
 public:
  using value_type = T;

  constexpr Vec2() noexcept = default;

  constexpr explicit Vec2(const value_type x, const value_type y) noexcept
  {
    m_data[0] = x;
    m_data[1] = y;
  }

  constexpr ~Vec2() noexcept = default;

  constexpr Vec2(Vec2&&) noexcept = default;
  constexpr Vec2(const Vec2&) noexcept = default;

  constexpr auto operator=(Vec2&&) noexcept -> Vec2& = default;
  constexpr auto operator=(const Vec2&) noexcept -> Vec2& = default;

  constexpr void set_x(const value_type x) noexcept
  {
    m_data[0] = x;
  }

  constexpr void set_y(const value_type y) noexcept
  {
    m_data[1] = y;
  }

  constexpr auto x() const noexcept -> value_type
  {
    return m_data[0];
  }

  constexpr auto y() const noexcept -> value_type
  {
    return m_data[1];
  }

  constexpr auto span() noexcept -> Span<value_type, 2>
  {
    return m_data;
  }

  constexpr auto span() const noexcept -> Span<const value_type, 2>
  {
    return m_data;
  }

  constexpr auto operator==(const Vec2&) const noexcept -> bool = default;

 private:
  Array<T, 2> m_data {};
};

using Vec2I = Vec2<int32>;
using Vec2L = Vec2<int64>;
using Vec2F = Vec2<float32>;
using Vec2D = Vec2<float64>;

template <Number T>
constexpr auto operator+(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept -> Vec2<T>
{
  return Vec2<T> {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
}

template <Number T>
constexpr auto operator-(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept -> Vec2<T>
{
  return Vec2<T> {lhs.x() - rhs.x(), lhs.y() - rhs.y()};
}

template <Number T>
constexpr auto operator*(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept -> Vec2<T>
{
  return Vec2<T> {lhs.x() * rhs.x(), lhs.y() * rhs.y()};
}

template <Number T>
constexpr auto operator*(const Vec2<T>& vec, const T scale) noexcept -> Vec2<T>
{
  return Vec2<T> {vec.x() * scale, vec.y() * scale};
}

}  // namespace tactile
