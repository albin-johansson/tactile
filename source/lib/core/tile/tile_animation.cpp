// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tile_animation.hpp"

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/debug/generic_error.hpp"

namespace tactile {

void TileAnimation::update()
{
  const auto now = SystemClock::now();
  const auto& current = current_frame();

  if (now - mLastUpdate >= current.duration) {
    mLastUpdate = now;
    mIndex = (mIndex + 1u) % mFrames.size();
  }
}

void TileAnimation::reserve_frames(const usize n)
{
  mFrames.reserve(n);
}

void TileAnimation::add_frame(const TileIndex tile, const Milliseconds duration)
{
  mFrames.push_back({tile, duration});
}

auto TileAnimation::insert_frame(const usize index,
                                 const TileIndex tile_index,
                                 const Milliseconds duration) -> Result<void>
{
  if (index == mFrames.size()) {
    add_frame(tile_index, duration);
    return kOK;
  }
  else if (index < mFrames.size()) {
    mFrames.insert(mFrames.begin() + static_cast<ssize>(index),
                   Frame {tile_index, duration});
    return kOK;
  }
  else {
    return unexpected(make_error(GenericError::kInvalidParam));
  }
}

auto TileAnimation::remove_frame(const usize frame_index) -> Result<void>
{
  if (frame_index < mFrames.size()) {
    const auto iter = mFrames.begin() + static_cast<ssize>(frame_index);
    mFrames.erase(iter);

    if (frame_index <= mIndex) {
      mIndex = 0;
      mLastUpdate = SystemClock::now();
    }

    return kOK;
  }
  else {
    return unexpected(make_error(GenericError::kInvalidParam));
  }
}

auto TileAnimation::operator[](const usize index) -> Frame&
{
  if (index < mFrames.size()) {
    return mFrames[index];
  }
  else {
    throw Exception {"Invalid frame index"};
  }
}

auto TileAnimation::operator[](const usize index) const -> const Frame&
{
  if (index < mFrames.size()) {
    return mFrames[index];
  }
  else {
    throw Exception {"Invalid frame index"};
  }
}

auto TileAnimation::current_frame() const -> const Frame&
{
  TACTILE_ASSERT(mIndex < mFrames.size());
  return mFrames.at(mIndex);
}

auto TileAnimation::capacity() const -> usize
{
  return mFrames.capacity();
}

auto TileAnimation::size() const -> usize
{
  return mFrames.size();
}

}  // namespace tactile
