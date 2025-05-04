// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module;

#include <cassert>

export module tactile.numeric:vec;

export import std;
export import tactile.core;
export import :concepts;
import :checked;

export namespace tactile {

/// Represents a mathematical vector.
template <Numeric T, usize N>
  requires(N >= 2 && N <= 4)
class Vec final
{
 public:
  using value_type = T;
  using size_type = usize;

  /// Creates a zero vector.
  constexpr Vec() noexcept = default;

  /// Creates a two-dimensional vector.
  constexpr Vec(const value_type x, const value_type y) noexcept
    requires(N == 2)
  {
    set_x(x);
    set_y(y);
  }

  /// Creates a three-dimensional vector.
  constexpr Vec(const value_type x, const value_type y, const value_type z) noexcept
    requires(N == 3)
  {
    set_x(x);
    set_y(y);
    set_z(z);
  }

  /// Creates a four-dimensional vector.
  constexpr Vec(const value_type x,
                const value_type y,
                const value_type z,
                const value_type w) noexcept
    requires(N == 4)
  {
    set_x(x);
    set_y(y);
    set_z(z);
    set_w(w);
  }

  /// Creates a four-dimensional vector from two two-dimensional vectors.
  constexpr Vec(const Vec<T, 2>& xy, const Vec<T, 2>& zw) noexcept
    requires(N == 4)
  {
    set_x(xy.x());
    set_y(xy.y());
    set_z(zw.x());
    set_w(zw.y());
  }

  constexpr Vec(const Vec&) noexcept = default;

  constexpr Vec(Vec&&) noexcept = default;

  constexpr ~Vec() noexcept = default;

  constexpr auto operator=(const Vec&) noexcept -> Vec& = default;

  constexpr auto operator=(Vec&&) noexcept -> Vec& = default;

  /// Sets the value of the x-coordinate.
  constexpr void set_x(const value_type x) noexcept
  {
    m_data[0] = x;
  }

  /// Sets the value of the y-coordinate.
  constexpr void set_y(const value_type y) noexcept
  {
    m_data[1] = y;
  }

  /// Sets the value of the z-coordinate.
  constexpr void set_z(const value_type z) noexcept
  {
    m_data[2] = z;
  }

  /// Sets the value of the w-coordinate.
  constexpr void set_w(const value_type w) noexcept
  {
    m_data[3] = w;
  }

  /// Returns the value of the x-coordinate.
  [[nodiscard]]
  constexpr auto x() const noexcept -> value_type
  {
    return m_data[0];
  }

  /// Returns the value of the y-coordinate.
  [[nodiscard]]
  constexpr auto y() const noexcept -> value_type
    requires(N > 1)
  {
    return m_data[1];
  }

  /// Returns the value of the z-coordinate.
  [[nodiscard]]
  constexpr auto z() const noexcept -> value_type
    requires(N > 2)
  {
    return m_data[2];
  }

  /// Returns the value of the w-coordinate.
  [[nodiscard]]
  constexpr auto w() const noexcept -> value_type
    requires(N > 3)
  {
    return m_data[3];
  }

  /// Returns the value of an element in the vector (without bounds checking).
  [[nodiscard]]
  constexpr auto operator[](const size_type index) const noexcept -> value_type
  {
    assert(index < N);
    return m_data[index];
  }

  /// Returns the value of an element in the vector (with bounds checking).
  [[nodiscard]]
  constexpr auto at(const size_type index) const -> value_type
  {
    if (index >= N) [[unlikely]] {
      throw std::out_of_range {"bad vector index"};
    }

    return m_data[index];
  }

  /// Returns a pointer to the first element of the internal array of elements.
  [[nodiscard]]
  auto data() noexcept -> value_type*
  {
    return m_data;
  }

  /// Returns a pointer to the first element of the internal array of elements.
  [[nodiscard]]
  auto data() const noexcept -> const value_type*
  {
    return m_data;
  }

  /// Returns the number of elements in the vector.
  [[nodiscard]]
  constexpr static auto size() noexcept -> size_type
  {
    return N;
  }

  [[nodiscard]]
  constexpr auto operator==(const Vec&) const noexcept -> bool = default;

  [[nodiscard]]
  constexpr auto operator<=>(const Vec&) const noexcept
      -> std::partial_ordering = default;

 private:
  value_type m_data[N] {};
};

template <Numeric T>
using Vec2 = Vec<T, 2>;

template <Numeric T>
using Vec3 = Vec<T, 3>;

template <Numeric T>
using Vec4 = Vec<T, 4>;

using Int2 = Vec2<i32>;
using Int3 = Vec3<i32>;
using Int4 = Vec4<i32>;

using Float2 = Vec2<f32>;
using Float3 = Vec3<f32>;
using Float4 = Vec4<f32>;

/// Creates a vector by transforming the elements from a vector.
template <Numeric T, usize N, std::invocable<T> Transform>
[[nodiscard]] constexpr auto elem_transform(const Vec<T, N>& vec,
                                            const Transform& transform)
    -> Vec<std::invoke_result_t<Transform, T>, N>
{
  Vec<std::invoke_result_t<Transform, T>, N> result {};

  result.set_x(transform(vec.x()));
  result.set_y(transform(vec.y()));

  if constexpr (N > 2) {
    result.set_z(transform(vec.z()));
  }

  if constexpr (N > 3) {
    result.set_w(transform(vec.w()));
  }

  return result;
}

/// Creates a vector by merging the elements from two vectors.
template <Numeric T, usize N, std::invocable<T, T> Merger>
[[nodiscard]] constexpr auto elem_merge(const Vec<T, N>& lhs,
                                        const Vec<T, N>& rhs,
                                        const Merger& merger)
    -> Vec<std::invoke_result_t<Merger, T, T>, N>
{
  Vec<std::invoke_result_t<Merger, T, T>, N> result {};

  result.set_x(merger(lhs.x(), rhs.x()));
  result.set_y(merger(lhs.y(), rhs.y()));

  if constexpr (N > 2) {
    result.set_z(merger(lhs.z(), rhs.z()));
  }

  if constexpr (N > 3) {
    result.set_w(merger(lhs.w(), rhs.w()));
  }

  return result;
}

/// Performs element-wise addition of two vectors.
template <Numeric T, usize N>
[[nodiscard]] constexpr auto operator+(const Vec<T, N>& lhs, const Vec<T, N>& rhs)
    -> Vec<T, N>
{
  return elem_merge(lhs, rhs, [](const T a, const T b) {
    return checked_add(a, b).value();
  });
}

/// Performs element-wise subtraction of two vectors.
template <Numeric T, usize N>
[[nodiscard]] constexpr auto operator-(const Vec<T, N>& lhs, const Vec<T, N>& rhs)
    -> Vec<T, N>
{
  return elem_merge(lhs, rhs, [](const T a, const T b) {
    return checked_sub(a, b).value();
  });
}

/// Performs element-wise multiplication of two vectors.
template <Numeric T, usize N>
[[nodiscard]] constexpr auto operator*(const Vec<T, N>& lhs, const Vec<T, N>& rhs)
    -> Vec<T, N>
{
  return elem_merge(lhs, rhs, [](const T a, const T b) {
    return checked_mul(a, b).value();
  });
}

/// Scales the elements of a vector by a given factor.
template <Numeric T, usize N>
[[nodiscard]] constexpr auto operator*(const Vec<T, N>& lhs, const T rhs)
    -> Vec<T, N>
{
  return elem_transform(lhs, [rhs](const T value) {
    return checked_mul(value, rhs).value();
  });
}

template <Numeric T, usize N>
constexpr auto operator+=(Vec<T, N>& lhs, const Vec<T, N>& rhs) -> Vec<T, N>&
{
  lhs = lhs + rhs;
  return lhs;
}

template <Numeric T, usize N>
constexpr auto operator-=(Vec<T, N>& lhs, const Vec<T, N>& rhs) -> Vec<T, N>&
{
  lhs = lhs - rhs;
  return lhs;
}

template <Numeric T, usize N>
constexpr auto operator*=(Vec<T, N>& lhs, const Vec<T, N>& rhs) -> Vec<T, N>&
{
  lhs = lhs * rhs;
  return lhs;
}

template <Numeric T, usize N>
constexpr auto operator*=(Vec<T, N>& lhs, const T rhs) -> Vec<T, N>&
{
  lhs = lhs * rhs;
  return lhs;
}

}  // namespace tactile
