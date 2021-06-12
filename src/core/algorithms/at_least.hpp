#pragma once

#include "ordered.hpp"

namespace tactile {

/**
 * \brief Clamps a value to be greater than or equal to the specified threshold.
 *
 * \tparam T the type of the values.
 *
 * \param value the value that will be clamped.
 * \param least the threshold that `value` must be greater than or equal to.
 *
 * \return `value` if `value` was greater than or equal to `least`; otherwise
 * `least` is returned.
 *
 * \since 0.1.0
 */
template <Ordered T>
[[nodiscard]] constexpr auto AtLeast(const T value, const T least) noexcept -> T
{
  return (value < least) ? least : value;
}

}  // namespace tactile
