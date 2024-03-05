/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tile_animation.hpp"

#include "common/debug/panic.hpp"
#include "tactile/core/debug/assert.hpp"

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
                                 const Milliseconds duration) -> Result
{
  if (index == mFrames.size()) {
    add_frame(tile_index, duration);
    return success;
  }
  else if (index < mFrames.size()) {
    mFrames.insert(mFrames.begin() + static_cast<ssize>(index),
                   Frame {tile_index, duration});
    return success;
  }
  else {
    return failure;
  }
}

auto TileAnimation::remove_frame(const usize frame_index) -> Result
{
  if (frame_index < mFrames.size()) {
    const auto iter = mFrames.begin() + static_cast<ssize>(frame_index);
    mFrames.erase(iter);

    if (frame_index <= mIndex) {
      mIndex = 0;
      mLastUpdate = SystemClock::now();
    }

    return success;
  }
  else {
    return failure;
  }
}

auto TileAnimation::operator[](const usize index) -> Frame&
{
  if (index < mFrames.size()) {
    return mFrames[index];
  }
  else {
    throw TactileError {"Invalid frame index"};
  }
}

auto TileAnimation::operator[](const usize index) const -> const Frame&
{
  if (index < mFrames.size()) {
    return mFrames[index];
  }
  else {
    throw TactileError {"Invalid frame index"};
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
