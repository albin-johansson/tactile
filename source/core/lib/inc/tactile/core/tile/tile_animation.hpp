// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/basic/clock.hpp"
#include "tactile/core/basic/primitives.hpp"
#include "tactile/core/basic/vector.hpp"
#include "tactile/core/fwd.hpp"

namespace tactile {

/// Describes a frame within a tile animation.
struct TACTILE_CORE_API TileAnimationFrame final
{
  /// The index of the tile shown during the frame.
  usize tile_index;

  /// The amount of time the frame is shown.
  Milliseconds duration;
};

/// A component type describing a tile animation.
struct TACTILE_CORE_API CTileAnimation final
{
  /// The time of the last frame transition.
  SteadyClock::time_point last_update;

  /// The index of the currently shown frame.
  usize frame_index;

  /// The sequence of frames the animation cycles through.
  Vector<TileAnimationFrame> frames;
};

/// Updates the state of a given tile animation.
TACTILE_CORE_API void update(CTileAnimation& animation,
                             SteadyClock::time_point reference_time);

/// Updates all tile animations in a given world.
TACTILE_CORE_API void update_tile_animations(World& world);

}  // namespace tactile
