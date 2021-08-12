#ifndef RUNE_MATH_MIN_HPP
#define RUNE_MATH_MIN_HPP

namespace rune {

/**
 * \brief Returns the smallest of two values.
 *
 * \note This function exists because `std::min()` isn't marked as `noexcept`.
 *
 * \ingroup math
 *
 * \tparam T the type of the values.
 *
 * \param a the first value.
 * \param b the second value.
 *
 * \return the smallest of the two values.
 */
template <typename T>
[[nodiscard]] constexpr auto min(const T& a, const T& b) noexcept(noexcept(a < b)) -> T
{
  return (a < b) ? a : b;
}

}  // namespace rune

#endif  // RUNE_MATH_MIN_HPP
