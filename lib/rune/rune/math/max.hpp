#ifndef RUNE_MATH_MAX_HPP
#define RUNE_MATH_MAX_HPP

#include <concepts>

namespace rune {

/**
 * \brief Returns the largest of two values.
 *
 * \note This function exists because `std::max()` isn't marked as `noexcept`.
 *
 * \note This function uses `operator<`, and not `operator>`.
 *
 * \ingroup math
 *
 * \tparam T the type of the values.
 *
 * \param a the first value.
 * \param b the second value.
 *
 * \return the largest of the two values.
 */
template <typename T>
[[nodiscard]] constexpr auto max(const T& a, const T& b) noexcept(noexcept(a < b)) -> T
{
  return (a < b) ? b : a;
}

}  // namespace rune

#endif  // RUNE_MATH_MAX_HPP
