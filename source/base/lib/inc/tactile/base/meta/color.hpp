// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents an 8-bit RGBA color.
 *
 * \see FColor
 */
struct UColor final
{
  using value_type = std::uint8_t;

  value_type red;
  value_type green;
  value_type blue;
  value_type alpha;

  [[nodiscard]]
  constexpr auto operator==(const UColor&) const noexcept -> bool = default;
};

/**
 * Represents a normalized floating-point RGBA color.
 *
 * \see UColor
 */
struct FColor final
{
  using value_type = float;

  value_type red;
  value_type green;
  value_type blue;
  value_type alpha;

  [[nodiscard]]
  constexpr auto operator==(const FColor&) const noexcept -> bool = default;
};

}  // namespace tactile
