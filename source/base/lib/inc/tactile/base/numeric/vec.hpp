// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cassert>      // assert
#include <compare>      // partial_ordering
#include <concepts>     // invocable
#include <cstddef>      // size_t
#include <stdexcept>    // out_of_range
#include <type_traits>  // invoke_result_t, is_same_v

#include "tactile/base/util/concepts.hpp"

namespace tactile {

/**
 * Represents a generic mathematical vector.
 *
 * \tparam T The type of the vector elements.
 * \tparam N The number of elements in the vector.
 */
template <ArithmeticType T, std::size_t N>
  requires(N >= 2 && N <= 4)
class Vec final
{
 public:
  using value_type = T;
  using size_type = std::size_t;

  /**
   * The number of elements in the vector.
   */
  constexpr static std::size_t kSize = N;

  /**
   * Creates a zero vector.
   */
  constexpr Vec() noexcept = default;

  /**
   * Creates a two-dimensional vector.
   *
   * \param x The initial x-coordinate.
   * \param y The initial y-coordinate.
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
   * \param x The initial x-coordinate.
   * \param y The initial y-coordinate.
   * \param z The initial z-coordinate.
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
   * \param x The initial x-coordinate.
   * \param y The initial y-coordinate.
   * \param z The initial z-coordinate.
   * \param w The initial w-coordinate.
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

  /**
   * Creates a four-dimensional vector from two two-dimensional vectors.
   *
   * \param a The vector that provides the x- and y-coordinates.
   * \param b The vector that provides the z- and w-coordinates.
   */
  constexpr Vec(const Vec<T, 2>& a, const Vec<T, 2>& b) noexcept
    requires(N == 4)
  {
    set_x(a.x());
    set_y(a.y());
    set_z(b.x());
    set_w(b.y());
  }

  constexpr ~Vec() noexcept = default;

  constexpr Vec(const Vec&) noexcept = default;

  constexpr Vec(Vec&&) noexcept = default;

  constexpr auto operator=(const Vec&) noexcept -> Vec& = default;

  constexpr auto operator=(Vec&&) noexcept -> Vec& = default;

  /**
   * Sets the value of the x-coordinate.
   *
   * \param x The new x-coordinate.
   */
  constexpr void set_x(const value_type x) noexcept
  {
    mData[0] = x;
  }

  /**
   * Sets the value of the y-coordinate.
   *
   * \param y The new y-coordinate.
   */
  constexpr void set_y(const value_type y) noexcept
  {
    mData[1] = y;
  }

  /**
   * Sets the value of the z-coordinate.
   *
   * \param z The new z-coordinate.
   */
  constexpr void set_z(const value_type z) noexcept
    requires(N > 2)
  {
    mData[2] = z;
  }

  /**
   * Sets the value of the w-coordinate.
   *
   * \param w The new w-coordinate.
   */
  constexpr void set_w(const value_type w) noexcept
    requires(N > 3)
  {
    mData[3] = w;
  }

  /**
   * Returns the value of an element in the vector (with bounds checking).
   *
   * \param index The index of the desired element.
   *
   * \return
   * The element at the given index.
   *
   * \throws Exception if the index is invalid.
   */
  [[nodiscard]]
  constexpr auto at(const size_type index) const -> value_type
  {
    if (index < N) [[likely]] {
      return mData[index];
    }

    throw std::out_of_range {"bad vector index"};
  }

  /**
   * Returns an element in the vector (without bounds checking).
   *
   * \pre The index must refer to an element in the vector.
   *
   * \param index The index of the desired element.
   *
   * \return
   * The element at the given index.
   */
  [[nodiscard]]
  constexpr auto operator[](const size_type index) noexcept -> value_type&
  {
    assert(index < N);
    return mData[index];
  }

  /** \copydoc operator[] */
  [[nodiscard]]
  constexpr auto operator[](const size_type index) const noexcept -> const value_type&
  {
    assert(index < N);
    return mData[index];
  }

  /**
   * Returns the value of the x-coordinate.
   *
   * \return
   * The x-coordinate.
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
   * The y-coordinate.
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
   * The z-coordinate.
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
   * The w-coordinate.
   */
  [[nodiscard]]
  constexpr auto w() const noexcept -> value_type
    requires(N > 3)
  {
    return mData[3];
  }

  /**
   * Returns a pointer to the internal array representation.
   *
   * \return
   * A pointer to the first element.
   */
  [[nodiscard]]
  constexpr auto data() noexcept -> value_type*
  {
    return mData;
  }

  /**
   * \copydoc data
   */
  [[nodiscard]]
  constexpr auto data() const noexcept -> const value_type*
  {
    return mData;
  }

  /**
   * Returns the number of elements in the vector.
   *
   * \return
   * An element count.
   */
  [[nodiscard]]
  constexpr auto size() noexcept -> size_type
  {
    return N;
  }

  /**
   * Produces a vector with the same magnitude but opposite direction.
   *
   * \return
   * A negated vector.
   */
  [[nodiscard]]
  constexpr auto operator-() const noexcept -> Vec
  {
    Vec negation = *this;

    for (value_type& coordinate : negation.mData) {
      coordinate = -coordinate;
    }

    return negation;
  }

  [[nodiscard]]
  constexpr auto operator==(const Vec&) const noexcept -> bool = default;

  [[nodiscard]]
  constexpr auto operator<=>(const Vec&) const noexcept -> std::partial_ordering = default;

 private:
  value_type mData[N] {};  // NOLINT(*-avoid-c-arrays)
};

using Float2 = Vec<float, 2>;
using Float3 = Vec<float, 3>;
using Float4 = Vec<float, 4>;

using Int2 = Vec<int, 2>;
using Int3 = Vec<int, 3>;
using Int4 = Vec<int, 4>;

/**
 * Creates a vector by applying a given function object to each coordinate of a
 * vector.
 *
 * \tparam T The element type used by the vector.
 * \tparam N The number of elements in the vector.
 * \tparam M A function object type.
 *
 * \param vec      The original vector.
 * \param modifier A function object that is applied to each coordinate in the
 *                 vector.
 *
 * \return
 * A vector.
 */
template <ArithmeticType T, std::size_t N, std::invocable<T> M>
[[nodiscard]] constexpr auto apply(const Vec<T, N>& vec, const M& modifier) noexcept
    -> Vec<std::invoke_result_t<M, T>, N>
{
  Vec<std::invoke_result_t<M, T>, N> result {};

  result[0] = modifier(vec[0]);
  result[1] = modifier(vec[1]);

  if constexpr (N > 2) {
    result[2] = modifier(vec[2]);
  }

  if constexpr (N > 3) {
    result[3] = modifier(vec[3]);
  }

  return result;
}

/**
 * Creates a vector using given function object on each coordinate pair in two
 * vectors.
 *
 * \tparam T The element type used by the vectors.
 * \tparam N The number of elements in the vectors.
 * \tparam C A function object type.
 *
 * \param lhs      The left-hand side vector.
 * \param rhs      The right-hand side vector.
 * \param combiner A function object that is applied to each coordinate pair.
 *
 * \return
 * A vector.
 */
template <ArithmeticType T, std::size_t N, std::invocable<T, T> C>
[[nodiscard]] constexpr auto apply2(const Vec<T, N>& lhs,
                                    const Vec<T, N>& rhs,
                                    const C& combiner) noexcept
    -> Vec<std::invoke_result_t<C, T, T>, N>
{
  Vec<std::invoke_result_t<C, T, T>, N> result {};

  result[0] = combiner(lhs[0], rhs[0]);
  result[1] = combiner(lhs[1], rhs[1]);

  if constexpr (N > 2) {
    result[2] = combiner(lhs[2], rhs[2]);
  }

  if constexpr (N > 3) {
    result[3] = combiner(lhs[3], rhs[3]);
  }

  return result;
}

/**
 * Converts a vector to a vector using a different element type.
 *
 * \tparam ToVec   The target vector type.
 * \tparam FromVec The source vector type.
 *
 * \param from The vector to convert.
 *
 * \return
 * A new vector.
 */
template <typename VecType, ArithmeticType T, std::size_t N>
  requires(VecType::kSize == N && !std::is_same_v<VecType, Vec<T, N>>)
[[nodiscard]] constexpr auto vec_cast(const Vec<T, N>& from) noexcept -> VecType
{
  return apply(from, [](const T value) { return static_cast<VecType::value_type>(value); });
}

#define TACTILE_VEC_IMPL_ARITHMETIC_OP(Op)                                                   \
  template <ArithmeticType T, std::size_t N>                                                 \
  [[nodiscard]] constexpr auto operator Op(const Vec<T, N>& lhs,                             \
                                           const Vec<T, N>& rhs) noexcept -> Vec<T, N>       \
  {                                                                                          \
    return apply2(lhs, rhs, [=](const T a, const T b) -> T { return a Op b; });              \
  }                                                                                          \
                                                                                             \
  template <ArithmeticType T, std::size_t N>                                                 \
  constexpr auto operator Op##=(Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>& \
  {                                                                                          \
    lhs = lhs Op rhs;                                                                        \
    return lhs;                                                                              \
  }                                                                                          \
  static_assert(true)

#define TACTILE_VEC_IMPL_SCALAR_OP(Op)                                                    \
  template <ArithmeticType T, std::size_t N>                                              \
  [[nodiscard]] constexpr auto operator Op(const Vec<T, N>& vec, const T scalar) noexcept \
      -> Vec<T, N>                                                                        \
  {                                                                                       \
    return apply(vec, [=](const T value) -> T { return value Op scalar; });               \
  }                                                                                       \
                                                                                          \
  template <ArithmeticType T, std::size_t N>                                              \
  [[nodiscard]] constexpr auto operator Op(const T scalar, const Vec<T, N>& vec) noexcept \
      -> Vec<T, N>                                                                        \
  {                                                                                       \
    return vec * scalar;                                                                  \
  }                                                                                       \
                                                                                          \
  template <ArithmeticType T, std::size_t N>                                              \
  constexpr auto operator Op##=(Vec<T, N>& vec, const T scalar) noexcept -> Vec<T, N>&    \
  {                                                                                       \
    vec = vec Op scalar;                                                                  \
    return vec;                                                                           \
  }                                                                                       \
  static_assert(true)

TACTILE_VEC_IMPL_ARITHMETIC_OP(+);
TACTILE_VEC_IMPL_ARITHMETIC_OP(-);
TACTILE_VEC_IMPL_ARITHMETIC_OP(*);
TACTILE_VEC_IMPL_ARITHMETIC_OP(/);

TACTILE_VEC_IMPL_SCALAR_OP(*);
TACTILE_VEC_IMPL_SCALAR_OP(/);

}  // namespace tactile
