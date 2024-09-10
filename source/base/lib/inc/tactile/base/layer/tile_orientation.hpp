// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents the different supported tile orientations.
 */
enum class TileOrientation : std::uint8_t
{
  /** The classic tile orientation, with orthogonal x and y axes. */
  kOrthogonal,

  /** Hexagons arranged so that one vertex is located directly above center. */
  kHexagonal,
};

}  // namespace tactile
