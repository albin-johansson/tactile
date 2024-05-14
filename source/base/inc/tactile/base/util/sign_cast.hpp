// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>     // signed_integral, unsigned_integral
#include <type_traits>  // make_unsigned_t, make_signed_t

#include "tactile/base/prelude.hpp"

namespace tactile {

template <std::signed_integral T>
[[nodiscard]] constexpr auto sign_cast(const T value) noexcept
    -> std::make_unsigned_t<T>
{
  return static_cast<std::make_unsigned_t<T>>(value);
}

template <std::unsigned_integral T>
[[nodiscard]] constexpr auto sign_cast(const T value) noexcept
    -> std::make_signed_t<T>
{
  return static_cast<std::make_signed_t<T>>(value);
}

}  // namespace tactile
