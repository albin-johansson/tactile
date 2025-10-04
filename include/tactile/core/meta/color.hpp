// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include "tactile/core/primitives.hpp"

namespace tactile {

struct Color final
{
  uint8 red {0U};
  uint8 blue {0U};
  uint8 green {0U};
  uint8 alpha {255U};

  auto operator==(const Color&) const noexcept -> bool = default;
};

}  // namespace tactile
