// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tile/tile_animation.hpp"

namespace tactile {

void TileAnimation::update(const SteadyClock::time_point reference_time)
{
  const auto& current_frame = mFrames.at(mFrameIndex);

  if (reference_time - mLastUpdate >= current_frame.duration) {
    const auto next_index = (mFrameIndex + 1) % mFrames.size();

    mLastUpdate = reference_time;
    mFrameIndex = next_index;
  }
}

}  // namespace tactile
