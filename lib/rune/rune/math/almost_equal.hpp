#ifndef RUNE_MATH_ALMOST_EQUAL_HPP
#define RUNE_MATH_ALMOST_EQUAL_HPP

#include <cmath>     // abs
#include <concepts>  // floating_point

namespace rune {

/// \addtogroup math
/// \{

// clang-format off

/// The default epsilon value used for floating point comparisons.
inline constexpr float default_epsilon = 0.00001f;

/**
 * \brief Indicates whether or not two floating-point values are almost equal.
 *
 * \details The two values are considered equal if the absolute value of their difference
 * is in the range [0, `epsilon`).
 *
 * \param a the first value.
 * \param b the second value.
 * \param epsilon the epsilon value.
 *
 * \return `true` if the values are almost equal; `false` otherwise.
 */
template <std::floating_point T>
[[nodiscard]] auto almost_equal(const T a,
                                const T b,
                                const T epsilon = default_epsilon) noexcept(noexcept(std::abs(a)))
    -> bool
{
  return std::abs(a - b) < epsilon;
}

// clang-format on

/// \} End of group math

}  // namespace rune

#endif  // RUNE_MATH_ALMOST_EQUAL_HPP
