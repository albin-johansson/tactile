// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // integral, signed_integral, unsigned_integral, floating_point

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/numeric/narrow.hpp"

namespace tactile {

/**
 * Creates a string using the native filesystem character type.
 *
 * \param str The source string.
 *
 * \return
 * A native string if successful; an error code otherwise.
 */
[[nodiscard]]
auto to_native_string(StringView str) -> Result<NativeString>;

/**
 * Converts a native string to a normal string.
 *
 * \param str The source string.
 *
 * \return
 * A normal string if successful; an error code otherwise.
 */
[[nodiscard]]
auto from_native_string(NativeStringView str) -> Result<String>;

/**
 * Attempts to convert a string to an integer.
 *
 * \param str  The source string.
 * \param base The numerical base to use.
 *
 * \return
 * An integer if successful; an error code otherwise.
 */
[[nodiscard]]
auto parse_int(StringView str, int base = 10) -> Result<int64>;

/**
 * \copydoc parse_int()
 */
[[nodiscard]]
auto parse_uint(StringView str, int base = 10) -> Result<uint64>;

/**
 * Attempts to convert a string to a floating-point value.
 *
 * \param str The source string.
 *
 * \return
 * A float if successful; an error code otherwise.
 */
[[nodiscard]]
auto parse_float(StringView str) -> Result<double>;

/**
 * Attempts to convert a string to a number.
 *
 * \tparam T An integral or floating-point type.
 *
 * \param str The source string.
 *
 * \return
 * A number if successful; an error code otherwise.
 */
template <typename T>
  requires(std::integral<T> || std::floating_point<T>)
[[nodiscard]] auto parse(const StringView str) -> Result<T>
{
  if constexpr (std::signed_integral<T>) {
    return parse_int(str).transform(
        [](int64 value) { return narrow_cast<T>(value); });
  }
  else if constexpr (std::unsigned_integral<T>) {
    return parse_uint(str).transform(
        [](uint64 value) { return narrow_cast<T>(value); });
  }
  else {
    return parse_float(str).transform(
        [](double value) { return narrow_cast<T>(value); });
  }
}

}  // namespace tactile
