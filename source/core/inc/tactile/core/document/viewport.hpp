// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

/// \addtogroup viewport
/// \{

/**
 * A component that represents a viewport over a tile grid.
 */
struct CViewport final
{
  /** The position of the top-left corner. */
  Float2 origin;

  /** The size of the viewport. */
  Float2 size;

  /** The content scale factor. */
  float scale;
};

/**
 * A component that provides constraints for a viewport.
 */
struct CViewportLimits final
{
  /** The minimum X and Y coordinates of the viewport. */
  Float2 min_origin;

  /** The maximum X and Y coordinates of the viewport. */
  Float2 max_origin;
};

/// \}

}  // namespace tactile
