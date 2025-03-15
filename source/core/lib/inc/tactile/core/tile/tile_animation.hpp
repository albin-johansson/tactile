// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/basic/clock.hpp"
#include "tactile/core/basic/primitives.hpp"
#include "tactile/core/basic/vector.hpp"

namespace tactile {

/// Describes a frame within a tile animation.
struct TACTILE_CORE_API TileAnimationFrame final
{
  /// The index of the tile shown during the frame.
  usize tile_index;

  /// The amount of time the frame is shown.
  Milliseconds duration;
};

/// Represents a tile animation.
class TACTILE_CORE_API TileAnimation final
{
 public:
  /// Updates the state of the tile animation.
  void update(SteadyClock::time_point reference_time);

 private:
  /// The time of the last frame transition.
  SteadyClock::time_point mLastUpdate {SteadyClock::now()};

  /// The index of the currently shown frame.
  usize mFrameIndex {0};

  /// The sequence of frames the animation cycles through.
  Vector<TileAnimationFrame> mFrames {};
};

}  // namespace tactile
