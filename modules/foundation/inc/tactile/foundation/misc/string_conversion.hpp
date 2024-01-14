// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // signed_integral, unsigned_integral, floating_point

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/misc/integer_conversion.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Converts a string into an unsigned 64-bit integer.
 *
 * \param str  The source string.
 * \param base The numerical base.
 *
 * \return
 *    The converted value, or nothing if an error occurred.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto str_to_u64(StringView str, int base = 10) -> Maybe<uint64>;

/**
 * Converts a string into a signed 64-bit integer.
 *
 * \param str  The source string.
 * \param base The numerical base.
 *
 * \return
 *    The converted value, or nothing if an error occurred.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto str_to_i64(StringView str, int base = 10) -> Maybe<int64>;

/**
 * Converts a string into a 64-bit float.
 *
 * \param str The source string.
 *
 * \return
 *    The converted value, or nothing if an error occurred.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto str_to_f64(StringView str) -> Maybe<float64>;

/**
 * Converts a string into multiple integers.
 *
 * \details
 *    This function can be used to efficiently extract integers stored in a string
 *    that are delimited by a given character separator. For example, the string
 *    `"1:2:3:4"` can be converted to the vector `{1, 2, 3, 4}` using a single call
 *    to this function.
 *
 * \param str       The source string.
 * \param separator The character used to delimit each integer.
 * \param base      The numerical base.
 *
 * \return
 *    The parsed integers. An empty vector is return if an error occurred.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto str_to_multiple_i32(StringView str,
                                                char separator,
                                                int base = 10) -> Vector<int32>;

/** \copydoc str_to_multiple_i32() */
[[nodiscard]]
TACTILE_FOUNDATION_API auto str_to_multiple_u32(StringView str,
                                                char separator,
                                                int base = 10) -> Vector<uint32>;

/**
 * Converts a string into multiple floats.
 *
 * \param str       The source string.
 * \param separator The character used to delimit each float.
 *
 * \return
 *    The parsed numbers. An empty vector is return if an error occurred.
 *
 * \see `str_to_multiple_i32`
 * \see `str_to_multiple_u32`
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto str_to_multiple_f32(StringView str, char separator)
    -> Vector<float32>;

/**
 * Converts a string into an unsigned integer value.
 *
 * \param str The source string.
 *
 * \return
 *    An unsigned integer.
 */
template <std::unsigned_integral T>
[[nodiscard]] auto str_to(const StringView str, const int base = 10) -> Maybe<T>
{
  if (const auto value = str_to_u64(str, base)) {
    return narrow<T>(*value);
  }

  return kNothing;
}

/**
 * Converts a string into a signed integer value.
 *
 * \param str The source string.
 *
 * \return
 *    A signed integer.
 */
template <std::signed_integral T>
[[nodiscard]] auto str_to(const StringView str, const int base = 10) -> Maybe<T>
{
  if (const auto value = str_to_i64(str, base)) {
    return narrow<T>(*value);
  }

  return kNothing;
}

/**
 * Converts a string into a floating-point value.
 *
 * \param str The source string.
 *
 * \return
 *    A float.
 */
template <std::floating_point T>
[[nodiscard]] auto str_to(const StringView str) -> Maybe<T>
{
  if (const auto value = str_to_f64(str)) {
    return static_cast<T>(*value);
  }

  return kNothing;
}

}  // namespace tactile