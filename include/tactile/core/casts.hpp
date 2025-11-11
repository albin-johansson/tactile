// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <stdexcept>
#include <utility>

#include "tactile/core/concepts.hpp"
#include "tactile/core/container/option.hpp"

namespace tactile {

/// Performs a type conversion guaranteed to be lossless at compile-time.
///
/// \param[in] from
///            The value to convert.
/// \returns   The converted value.
template <typename To, TriviallyConvertible<To> From>
constexpr auto trivial_cast(const From from) noexcept -> To
{
  return static_cast<To>(from);
}

/// Performs a non-throwing lossless type conversion.
///
/// \param[in] from
///            The value to convert.
/// \returns   The converted value. An empty optional is returned if the conversion would
///            be lossy.
template <Integer To, Integer From>
constexpr auto try_convert_to(const From from) noexcept -> Option<To>
{
  if constexpr (!TriviallyConvertible<From, To>) {
    if (!std::in_range<To>(from)) [[unlikely]] {
      return kNone;
    }
  }

  return static_cast<To>(from);
}

/// Performs a checked lossless type conversion.
///
/// \param[in] from
///            The value to convert.
/// \returns   The converted value.
///
/// \throws    std::range_error
///            If the conversion is lossy.
template <Integer To, Integer From>
constexpr auto convert_to(const From from) -> To
{
  if constexpr (TriviallyConvertible<From, To>) {
    return static_cast<To>(from);
  }
  else {
    // We don't use .value() here to make the error message less cryptic.
    const auto to = try_convert_to<To>(from);

    if (!to.has_value()) [[unlikely]] {
      throw std::range_error {"Unexpected lossy conversion"};
    }

    return *to;
  }
}

}  // namespace tactile
