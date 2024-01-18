// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>  // min, max
#include <compare>    // strong_ordering
#include <concepts>   // same_as, integral, floating_point
#include <ostream>    // ostream

#include "tactile/foundation/container/array.hpp"
#include "tactile/foundation/debug/assert.hpp"
#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Describes numeric types, that is, types that primarily represent numeric values.
 *
 * \tparam T The type to check.
 */
template <typename T>
concept Numeric = !std::same_as<T, bool> && (std::integral<T> || std::floating_point<T>);

/**
 * Represents a generic mathematical vector.
 *
 * \tparam T The type of the vector elements.
 * \tparam N The number of elements in the vector.
 *
 * \see `vector_cast`
 * \see `compare_elements(const Vec&, const Vec&)`
 * \see `min(const Vec&, const Vec&)`
 * \see `max(const Vec&, const Vec&)`
 */
template <Numeric T, usize N>
  requires(N >= 2 && N <= 4)
class Vec final {
 public:
  using value_type = T;
  using size_type = usize;

  /**
   * Creates a vector of all zeroes.
   */
  constexpr Vec() noexcept = default;

  /**
   * Creates a two-dimensional vector.
   *
   * \param x The initial x-coordinate value.
   * \param y The initial y-coordinate value.
   */
  constexpr Vec(const value_type x, const value_type y) noexcept
    requires(N == 2)
  {
    set_x(x);
    set_y(y);
  }

  /**
   * Creates a three-dimensional vector.
   *
   * \param x The initial x-coordinate value.
   * \param y The initial y-coordinate value.
   * \param z The initial z-coordinate value.
   */
  constexpr Vec(const value_type x, const value_type y, const value_type z) noexcept
    requires(N == 3)
  {
    set_x(x);
    set_y(y);
    set_z(z);
  }

  /**
   * Creates a four-dimensional vector.
   *
   * \param x The initial x-coordinate value.
   * \param y The initial y-coordinate value.
   * \param z The initial z-coordinate value.
   * \param w The initial w-coordinate value.
   */
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

  constexpr ~Vec() noexcept = default;

  constexpr Vec(const Vec&) noexcept = default;
  constexpr Vec(Vec&&) noexcept = default;

  constexpr auto operator=(const Vec&) noexcept -> Vec& = default;
  constexpr auto operator=(Vec&&) noexcept -> Vec& = default;

  /**
   * Updates the value of the x-coordinate.
   *
   * \param x The new x-coordinate value.
   */
  constexpr void set_x(const value_type x) noexcept { mData[0] = x; }

  /**
   * Updates the value of the y-coordinate.
   *
   * \param y The new y-coordinate value.
   */
  constexpr void set_y(const value_type y) noexcept { mData[1] = y; }

  /**
   * Updates the value of the z-coordinate.
   *
   * \param z The new z-coordinate value.
   */
  constexpr void set_z(const value_type z) noexcept
    requires(N > 2)
  {
    mData[2] = z;
  }

  /**
   * Updates the value of the w-coordinate.
   *
   * \param w The new w-coordinate value.
   */
  constexpr void set_w(const value_type w) noexcept
    requires(N > 3)
  {
    mData[3] = w;
  }

  /**
   * Returns the value of an element in the vector.
   *
   * \param index The index of the desired element.
   *
   * \return
   *    The element at the given index.
   *
   * \throws Exception if the index is invalid.
   */
  [[nodiscard]] constexpr auto at(const size_type index) const -> value_type
  {
    if (index < N) {
      return mData[index];
    }

    throw Exception {"bad vector index"};
  }

  /**
   * Returns an element in the vector.
   *
   * \pre The index must refer to an element in the vector.
   *
   * \param index The index of the desired element.
   *
   * \return
   *    The element at the given index.
   */
  [[nodiscard]] constexpr auto operator[](const size_type index) noexcept -> value_type&
  {
    TACTILE_ASSERT(index < N);
    return mData[index];
  }

  /** \copydoc operator[] */
  [[nodiscard]] constexpr auto operator[](const size_type index) const noexcept
      -> const value_type&
  {
    TACTILE_ASSERT(index < N);
    return mData[index];
  }

  /**
   * Returns the value of the x-coordinate.
   *
   * \return
   *    The x-coordinate.
   */
  [[nodiscard]]
  constexpr auto x() const noexcept -> value_type
  {
    return mData[0];
  }

  /**
   * Returns the value of the y-coordinate.
   *
   * \return
   *    The y-coordinate.
   */
  [[nodiscard]]
  constexpr auto y() const noexcept -> value_type
  {
    return mData[1];
  }

  /**
   * Returns the value of the z-coordinate.
   *
   * \return
   *    The z-coordinate.
   */
  [[nodiscard]]
  constexpr auto z() const noexcept -> value_type
    requires(N > 2)
  {
    return mData[2];
  }

  /**
   * Returns the value of the w-coordinate.
   *
   * \return
   *    The w-coordinate.
   */
  [[nodiscard]]
  constexpr auto w() const noexcept -> value_type
    requires(N > 3)
  {
    return mData[3];
  }

  /**
   * Returns a pointer to the internal array representation of the vector.
   *
   * \return
   *    A pointer to the first element.
   */
  [[nodiscard]]
  constexpr auto data() noexcept -> value_type*
  {
    return mData.data();
  }

  /** \copydoc data() */
  [[nodiscard]]
  constexpr auto data() const noexcept -> const value_type*
  {
    return mData.data();
  }

  /**
   * Returns the number of elements in the vector.
   *
   * \return
   *    The number of elements.
   */
  [[nodiscard]]
  static constexpr auto size() noexcept -> size_type
  {
    return N;
  }

  /**
   * Produces a vector with the same magnitude but opposite direction.
   *
   * \return
   *    A negated vector.
   */
  [[nodiscard]] constexpr auto operator-() const noexcept -> Vec
  {
    Vec negation = *this;

    for (value_type& value : negation.mData) {
      value = -value;
    }

    return negation;
  }

  [[nodiscard]] constexpr auto operator==(const Vec&) const noexcept -> bool = default;

  [[nodiscard]] constexpr auto operator<=>(const Vec&) const noexcept
      -> std::strong_ordering = default;

 private:
  Array<T, N> mData {};
};

using Int2 = Vec<int, 2>;
using Int3 = Vec<int, 3>;
using Int4 = Vec<int, 4>;

using Float2 = Vec<float, 2>;
using Float3 = Vec<float, 3>;
using Float4 = Vec<float, 4>;

/**
 * Converts a vector to a vector using a different element type.
 *
 * \tparam T The element type used by the new vector.
 * \tparam U The element type used by the provided vector.
 * \tparam N The number of elements in the vector.
 *
 * \param vec The vector to convert.
 *
 * \return
 *    A vector obtained by casting each element in the provided vector to the specified type.
 */
template <Numeric T, Numeric U, usize N>
[[nodiscard]] constexpr auto vector_cast(const Vec<U, N>& vec) noexcept -> Vec<T, N>
{
  Vec<T, N> result {};

  for (usize idx = 0; idx < N; ++idx) {
    result[idx] = static_cast<T>(vec[idx]);
  }

  return result;
}

/**
 * Creates a vector that features the smallest elements from two given vectors.
 *
 * \tparam T The element type used by the new vector.
 * \tparam N The element type used by the provided vector.
 *
 * \param lhs The left-hand side vector.
 * \param rhs The right-hand side vector.
 *
 * \return
 *    A vector with the smallest elements from the respective vectors.
 */
template <Numeric T, usize N>
[[nodiscard]] constexpr auto min(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    -> Vec<T, N>
{
  Vec<T, N> result {};

  for (usize idx = 0; idx < N; ++idx) {
    result[idx] = std::min(lhs[idx], rhs[idx]);
  }

  return result;
}

/**
 * Creates a vector that features the largest elements from two given vectors.
 *
 * \tparam T The element type used by the new vector.
 * \tparam N The element type used by the provided vector.
 *
 * \param lhs The left-hand side vector.
 * \param rhs The right-hand side vector.
 *
 * \return
 *    A vector with the largest elements from the respective vectors.
 */
template <Numeric T, usize N>
[[nodiscard]] constexpr auto max(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    -> Vec<T, N>
{
  Vec<T, N> result {};

  for (usize idx = 0; idx < N; ++idx) {
    result[idx] = std::max(lhs[idx], rhs[idx]);
  }

  return result;
}

/**
 * Represents the result from an element-wise vector comparison.
 */
struct VecComparisonResult final {
  bool same_x : 1 {};
  bool same_y : 1 {};
  bool same_z : 1 {};
  bool same_w : 1 {};
};

/**
 * Performs an element-wise comparison of two vectors.
 *
 * \tparam T The element type used by both vectors.
 * \tparam N The number of elements in both vectors.
 *
 * \param lhs The left-hand side vector.
 * \param rhs The right-hand side vector.
 *
 * \return
 *    The element-wise comparison result.
 */
template <Numeric T, usize N>
[[nodiscard]] constexpr auto compare_elements(const Vec<T, N>& lhs,
                                              const Vec<T, N>& rhs) noexcept
    -> VecComparisonResult
{
  VecComparisonResult result {};
  result.same_x = lhs[0] == rhs[0];
  result.same_y = lhs[1] == rhs[1];

  if constexpr (N > 2) {
    result.same_z = lhs[2] == rhs[2];
  }

  if constexpr (N > 3) {
    result.same_w = lhs[3] == rhs[3];
  }

  return result;
}

/**
 * Outputs a vector to a stream (for debugging purposes).
 *
 * \tparam T The element type used by the vector.
 * \tparam N The number of elements in the vector.
 *
 * \param stream The output stream.
 * \param vec    The vector to output.
 *
 * \return
 *    The output stream.
 */
template <Numeric T, usize N>
auto operator<<(std::ostream& stream, const Vec<T, N>& vec) noexcept -> std::ostream&
{
  stream << '(';

  for (usize idx = 0; idx < N; ++idx) {
    if (idx != 0) {
      stream << ';';
    }

    stream << vec[idx];
  }

  stream << ')';

  return stream;
}

#define TACTILE_IMPLEMENT_VECTOR2_ARITHMETIC_OP(Op)                                    \
  template <Numeric T>                                                                 \
  [[nodiscard]] constexpr auto operator Op(const Vec<T, 2>& lhs,                       \
                                           const Vec<T, 2>& rhs) noexcept -> Vec<T, 2> \
  {                                                                                    \
    return {lhs.x() Op rhs.x(), lhs.y() Op rhs.y()};                                   \
  }                                                                                    \
                                                                                       \
  template <Numeric T>                                                                 \
  constexpr auto operator Op##=(Vec<T, 2>& lhs, const Vec<T, 2>& rhs) noexcept         \
      -> Vec<T, 2>&                                                                    \
  {                                                                                    \
    lhs = lhs Op rhs;                                                                  \
    return lhs;                                                                        \
  }

#define TACTILE_IMPLEMENT_VECTOR3_ARITHMETIC_OP(Op)                                    \
  template <Numeric T>                                                                 \
  [[nodiscard]] constexpr auto operator Op(const Vec<T, 3>& lhs,                       \
                                           const Vec<T, 3>& rhs) noexcept -> Vec<T, 3> \
  {                                                                                    \
    return {lhs.x() Op rhs.x(), lhs.y() Op rhs.y(), lhs.z() Op rhs.z()};               \
  }                                                                                    \
                                                                                       \
  template <Numeric T>                                                                 \
  constexpr auto operator Op##=(Vec<T, 3>& lhs, const Vec<T, 3>& rhs) noexcept         \
      -> Vec<T, 3>&                                                                    \
  {                                                                                    \
    lhs = lhs Op rhs;                                                                  \
    return lhs;                                                                        \
  }

#define TACTILE_IMPLEMENT_VECTOR4_ARITHMETIC_OP(Op)                                    \
  template <Numeric T>                                                                 \
  [[nodiscard]] constexpr auto operator Op(const Vec<T, 4>& lhs,                       \
                                           const Vec<T, 4>& rhs) noexcept -> Vec<T, 4> \
  {                                                                                    \
    return {lhs.x() Op rhs.x(),                                                        \
            lhs.y() Op rhs.y(),                                                        \
            lhs.z() Op rhs.z(),                                                        \
            lhs.w() Op rhs.w()};                                                       \
  }                                                                                    \
                                                                                       \
  template <Numeric T>                                                                 \
  constexpr auto operator Op##=(Vec<T, 4>& lhs, const Vec<T, 4>& rhs) noexcept         \
      -> Vec<T, 4>&                                                                    \
  {                                                                                    \
    lhs = lhs Op rhs;                                                                  \
    return lhs;                                                                        \
  }

TACTILE_IMPLEMENT_VECTOR2_ARITHMETIC_OP(+)
TACTILE_IMPLEMENT_VECTOR2_ARITHMETIC_OP(-)
TACTILE_IMPLEMENT_VECTOR2_ARITHMETIC_OP(*)
TACTILE_IMPLEMENT_VECTOR2_ARITHMETIC_OP(/)

TACTILE_IMPLEMENT_VECTOR3_ARITHMETIC_OP(+)
TACTILE_IMPLEMENT_VECTOR3_ARITHMETIC_OP(-)
TACTILE_IMPLEMENT_VECTOR3_ARITHMETIC_OP(*)
TACTILE_IMPLEMENT_VECTOR3_ARITHMETIC_OP(/)

TACTILE_IMPLEMENT_VECTOR4_ARITHMETIC_OP(+)
TACTILE_IMPLEMENT_VECTOR4_ARITHMETIC_OP(-)
TACTILE_IMPLEMENT_VECTOR4_ARITHMETIC_OP(*)
TACTILE_IMPLEMENT_VECTOR4_ARITHMETIC_OP(/)

}  // namespace tactile
