// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents the different supported map orientations.
 */
enum class MapOrientation : uint8
{
  /** Classic map orientation, with orthogonal x and y axes. */
  kOrthogonal,

  /** Map with hexagonal tiles. */
  kHexagonal,
};

}  // namespace tactile
