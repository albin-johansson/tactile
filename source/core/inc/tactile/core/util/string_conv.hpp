// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>      // integral, signed_integral, unsigned_integral, floating_point
#include <expected>      // expected
#include <string>        // string
#include <string_view>   // string_view
#include <system_error>  // error_code

#include "tactile/base/int.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/base/platform/native_string.hpp"

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
auto to_native_string(std::string_view str) -> std::expected<NativeString, std::error_code>;

/**
 * Converts a native string to a normal string.
 *
 * \param str The source string.
 *
 * \return
 * A normal string if successful; an error code otherwise.
 */
[[nodiscard]]
auto from_native_string(NativeStringView str) -> std::expected<std::string, std::error_code>;

/**
 * Attempts to convert a string to a floating-point value.
 *
 * \param str The source string.
 *
 * \return
 * A float if successful; an error code otherwise.
 */
[[nodiscard]]
auto parse_float(std::string_view str) -> std::expected<double, std::error_code>;

/**
 * Attempts to convert a string to a float.
 *
 * \tparam T A floating-point type.
 *
 * \param str The source string.
 *
 * \return
 * A float if successful; an error code otherwise.
 */
template <std::floating_point T>
[[nodiscard]] auto parse(const std::string_view str) -> std::expected<T, std::error_code>
{
  return parse_float(str).transform([](double value) { return static_cast<T>(value); });
}

}  // namespace tactile
