// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <chrono>   // milliseconds, steady_clock
#include <cstddef>  // size_t
#include <vector>   // vector

#include "tactile/base/id.hpp"

namespace tactile::core {

/**
 * Represents a single frame in a tile animation.
 */
struct AnimationFrame final
{
  /** The local identifier of the tile shown during the frame. */
  TileIndex tile_index;

  /** The amount of time the frame is active. */
  std::chrono::milliseconds duration;
};

/**
 * A component that represents a sequential tile animation.
 */
struct CAnimation final
{
  /** The time of the last frame update. */
  std::chrono::steady_clock::time_point last_update;

  /** The index of the currently displayed frame. */
  std::size_t frame_index;

  /** The sequence of frames the animation cycles through. */
  std::vector<AnimationFrame> frames;
};

}  // namespace tactile::core
