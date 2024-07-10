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
  uint8 red;
  uint8 green;
  uint8 blue;
  uint8 alpha;

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
  float red;
  float green;
  float blue;
  float alpha;

  [[nodiscard]]
  constexpr auto operator==(const FColor&) const noexcept -> bool = default;
};

}  // namespace tactile
