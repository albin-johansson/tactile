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

#include "viewport.hpp"

#include <glm/common.hpp>

#include "common/debug/assert.hpp"

namespace tactile {
namespace {

constexpr float viewport_min_tile_height = 4;

[[nodiscard]] auto viewport_offset_delta(const float tile_width, const float ratio)
    -> Float2
{
  const auto dx = glm::round((glm::max)(2.0f, tile_width * 0.05f));
  const auto dy = dx / ratio;
  return {dx, dy};
}

}  // namespace

void Viewport::reset_limits()
{
  mLimits.reset();
}

void Viewport::offset(const Float2& delta)
{
  mOffset += delta;

  if (mLimits) {
    mOffset = (glm::max)(mLimits->min_offset, mOffset);
    mOffset = (glm::min)(mLimits->max_offset, mOffset);
  }
}

void Viewport::pan_left()
{
  mOffset.x += mTileSize.x;
}

void Viewport::pan_right()
{
  mOffset.x -= mTileSize.x;
}

void Viewport::pan_up()
{
  mOffset.y += mTileSize.y;
}

void Viewport::pan_down()
{
  mOffset.y -= mTileSize.y;
}

void Viewport::zoom_in(const Float2& anchor)
{
  // Percentages of area to the left of and above the cursor
  const auto percentage = (anchor - mOffset) / mTileSize;

  mTileSize += viewport_offset_delta(mTileSize.x, mTileSize.x / mTileSize.y);
  mOffset = anchor - (percentage * mTileSize);
}

void Viewport::zoom_out(const Float2& anchor)
{
  // Percentages of area to the left of and above the cursor
  const auto percentage = (anchor - mOffset) / mTileSize;

  {
    const auto ratio = mTileSize.x / mTileSize.y;
    mTileSize -= viewport_offset_delta(mTileSize.x, ratio);

    const Float2 minimum {viewport_min_tile_height * ratio, viewport_min_tile_height};
    mTileSize = (glm::max)(minimum, mTileSize);
  }

  mOffset = anchor - (percentage * mTileSize);
}

void Viewport::set_tile_size(const Float2& size)
{
  TACTILE_ASSERT(size.x > 0);
  TACTILE_ASSERT(size.y > 0);
  mTileSize = size;
}

void Viewport::set_limits(const ViewportLimits& limits)
{
  mLimits = limits;
}

auto Viewport::can_zoom_out() const -> bool
{
  return mTileSize.y > viewport_min_tile_height;
}

auto Viewport::scaling_ratio(const Float2& tileSize) const -> Float2
{
  return mTileSize / tileSize;
}

}  // namespace tactile
