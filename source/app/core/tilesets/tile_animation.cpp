/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

void TileAnimation::update()
{
  const auto now = Clock::now();

  TACTILE_ASSERT(mIndex < mFrames.size());
  const auto& currentFrame = mFrames.at(mIndex);

  if (now - mLastUpdate >= currentFrame.duration) {
    mLastUpdate = now;
    mIndex = (mIndex + 1u) % mFrames.size();
  }
}

void TileAnimation::reserve_frames(const usize n)
{
  mFrames.reserve(n);
}

void TileAnimation::add_frame(const TileIndex tile, const Millis duration)
{
  mFrames.push_back({tile, duration});
}

auto TileAnimation::nth(const usize index) const -> const Frame&
{
  if (index < mFrames.size()) {
    return mFrames[index];
  }
  else {
    throw TactileError{"Invalid frame index!"};
  }
}

auto TileAnimation::current_tile() const -> TileIndex
{
  TACTILE_ASSERT(mIndex < mFrames.size());
  return mFrames.at(mIndex).tile;
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
