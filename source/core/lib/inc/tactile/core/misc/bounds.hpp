// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/math/vec.hpp"

namespace tactile {

/// A component type that represents a bounding box.
struct TACTILE_CORE_API CBounds final
{
  /// The position of the top-left corner.
  Float2 upper_left;

  /// The position of the bottom-right corner.
  Float2 lower_right;
};

}  // namespace tactile
