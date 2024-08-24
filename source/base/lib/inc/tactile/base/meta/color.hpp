// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents an 8-bit RGBA color.
 *
 * \see FColor
 */
struct UColor final
{
  using value_type = uint8;

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
