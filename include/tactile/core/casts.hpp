// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <utility>

#include "tactile/core/concepts.hpp"
#include "tactile/core/container/option.hpp"

namespace tactile {

/// Performs a type conversion guaranteed to be lossless at compile-time.
template <typename To, TriviallyConvertible<To> From>
constexpr auto trivial_cast(const From from) noexcept -> To
{
  return static_cast<To>(from);
}

/// Performs a checked type conversion.
template <Integer To, Integer From>
constexpr auto checked_cast(const From from) noexcept -> Option<To>
{
  if (!std::in_range<To>(from)) [[unlikely]] {
    return kNone;
  }

  return static_cast<To>(from);
}

}  // namespace tactile
