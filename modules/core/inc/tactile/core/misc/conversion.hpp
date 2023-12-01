// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>     // unsigned_integral, signed_integral
#include <type_traits>  // make_unsigned_t, make_signed_t

#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Converts an unsigned integer to a signed integer of the same width.
 *
 * \tparam T an unsigned integer type.
 *
 * \param value the value to convert.
 *
 * \return a signed integer.
 */
template <std::unsigned_integral T>
[[nodiscard]] constexpr auto as_signed(const T value) noexcept -> std::make_signed_t<T>
{
  return static_cast<std::make_signed_t<T>>(value);
}

/**
 * \brief Converts a signed integer to an unsigned integer of the same width.
 *
 * \tparam T a signed integer type.
 *
 * \param value the value to convert.
 *
 * \return an unsigned integer.
 */
template <std::signed_integral T>
[[nodiscard]] constexpr auto as_unsigned(const T value) noexcept
    -> std::make_unsigned_t<T>
{
  return static_cast<std::make_unsigned_t<T>>(value);
}

}  // namespace tactile
