// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>    // min, max
#include <cmath>        // round, floor, ceil
#include <concepts>     // invocable
#include <type_traits>  // invoke_result_t

#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/concepts.hpp"

namespace tactile {

/**
 * Creates a vector that features the absolute values of a given vector.
 *
 * \tparam T The element type used by the vector.
 * \tparam N The number of elements in the vector.
 *
 * \param vec The original vector.
 *
 * \return
 * A vector with absolute elements.
 */
template <ArithmeticType T, std::size_t N>
[[nodiscard]] constexpr auto abs(const Vec<T, N>& vec) noexcept -> Vec<T, N>
{
  return apply(vec, [](const T value) { return std::abs(value); });
}

/**
 * Creates a vector that features the rounded values of a given vector.
 *
 * \tparam T The element type used by the vector.
 * \tparam N The number of elements in the vector.
 *
 * \param vec The original vector.
 *
 * \return
 * A vector with rounded elements.
 */
template <ArithmeticType T, std::size_t N>
[[nodiscard]] constexpr auto round(const Vec<T, N>& vec) noexcept -> Vec<T, N>
{
  return apply(vec, [](const T value) { return std::round(value); });
}

/**
 * Creates a vector that features the floored coordinates of a given vector.
 *
 * \tparam T The element type used by the vector.
 * \tparam N The number of elements in the vector.
 *
 * \param vec The original vector.
 *
 * \return
 * A vector with floored elements.
 */
template <ArithmeticType T, std::size_t N>
[[nodiscard]] constexpr auto floor(const Vec<T, N>& vec) noexcept -> Vec<T, N>
{
  return apply(vec, [](const T value) { return std::floor(value); });
}

/**
 * Creates a vector with coordinates rounded upwards to the closest integer.
 *
 * \tparam T The element type used by the vector.
 * \tparam N The number of elements in the vector.
 *
 * \param vec The original vector.
 *
 * \return
 * A new vector.
 */
template <ArithmeticType T, std::size_t N>
[[nodiscard]] constexpr auto ceil(const Vec<T, N>& vec) noexcept -> Vec<T, N>
{
  return apply(vec, [](const T value) { return std::ceil(value); });
}

/**
 * Creates a vector that features the smallest elements from two given vectors.
 *
 * \tparam T The element type used by the vectors.
 * \tparam N The number of elements in the vectors.
 *
 * \param lhs The left-hand side vector.
 * \param rhs The right-hand side vector.
 *
 * \return
 * A vector with the smallest elements from the respective vectors.
 */
template <ArithmeticType T, std::size_t N>
[[nodiscard]] constexpr auto min(const Vec<T, N>& lhs,
                                 const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
  return apply2(lhs, rhs, [](const T a, const T b) { return std::min(a, b); });
}

/**
 * Creates a vector that features the largest elements from two given vectors.
 *
 * \tparam T The element type used by the vectors.
 * \tparam N The number of elements in the vectors.
 *
 * \param lhs The left-hand side vector.
 * \param rhs The right-hand side vector.
 *
 * \return
 * A vector with the largest elements from the respective vectors.
 */
template <ArithmeticType T, std::size_t N>
[[nodiscard]] constexpr auto max(const Vec<T, N>& lhs,
                                 const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
  return apply2(lhs, rhs, [](const T a, const T b) { return std::max(a, b); });
}

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
 * The element-wise comparison result.
 */
template <ArithmeticType T, std::size_t N>
[[nodiscard]] constexpr auto compare_elements(const Vec<T, N>& lhs,
                                              const Vec<T, N>& rhs) noexcept -> Vec<bool, N>
{
  return apply2(lhs, rhs, [](const T a, const T b) { return a == b; });
}

}  // namespace tactile
