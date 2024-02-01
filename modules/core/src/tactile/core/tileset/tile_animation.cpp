// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tileset/tile_animation.hpp"

#include "tactile/foundation/misc/integer_conversion.hpp"

namespace tactile::core {

using int_literals::operator""_uz;

void TileAnimation::update()
{
  const auto now = SystemClock::now();
  const auto time_since_last_update = now - mLastUpdate;

  const auto& current_frame = get_frame(mCurrentFrameIndex);
  if (time_since_last_update >= current_frame.duration) {
    mLastUpdate = now;
    mCurrentFrameIndex = (mCurrentFrameIndex + 1_uz) % mFrames.size();
  }
}

void TileAnimation::append_frame(const TileAnimationFrame& frame)
{
  mFrames.push_back(frame);
}

void TileAnimation::insert_frame(const usize index, const TileAnimationFrame& frame)
{
  if (index == mFrames.size()) {
    append_frame(frame);
  }
  else if (index < mFrames.size()) {
    mFrames.insert(mFrames.begin() + as_signed(index), frame);
  }
}

void TileAnimation::remove_frame(const usize index)
{
  if (index < mFrames.size()) {
    mFrames.erase(mFrames.begin() + as_signed(index));

    // Reset the animation if we removed a frame positioned before the current frame.
    if (index <= mCurrentFrameIndex) {
      mCurrentFrameIndex = 0_uz;
      mLastUpdate = SystemClock::now();
    }
  }
}

auto TileAnimation::get_current_frame_index() const -> usize
{
  return mCurrentFrameIndex;
}

auto TileAnimation::get_frame(const usize index) -> TileAnimationFrame&
{
  return mFrames.at(index);
}

auto TileAnimation::get_frame(const usize index) const -> const TileAnimationFrame&
{
  return mFrames.at(index);
}

auto TileAnimation::frame_count() const -> usize
{
  return mFrames.size();
}

}  // namespace tactile::core
