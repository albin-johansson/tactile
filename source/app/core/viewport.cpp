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

#include <cmath>  // round

#include <glm/common.hpp>

#include "misc/assert.hpp"

namespace tactile {
namespace {

constexpr float _min_tile_height = 4;

[[nodiscard]] auto _viewport_offset_delta(const float tileWidth, const float ratio)
    -> Vector2f
{
  const auto dx = std::round((std::max)(2.0f, tileWidth * 0.05f));
  const auto dy = dx / ratio;
  return {dx, dy};
}

}  // namespace

void Viewport::reset_limits()
{
  mLimits.reset();
}

void Viewport::offset(const Vector2f& delta)
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

void Viewport::zoom_in(const Vector2f& anchor)
{
  // Percentages of area to the left of and above the cursor
  const auto percentage = (anchor - mOffset) / mTileSize;

  mTileSize += _viewport_offset_delta(mTileSize.x, mTileSize.x / mTileSize.y);
  mOffset = anchor - (percentage * mTileSize);
}

void Viewport::zoom_out(const Vector2f& anchor)
{
  // Percentages of area to the left of and above the cursor
  const auto percentage = (anchor - mOffset) / mTileSize;

  {
    const auto ratio = mTileSize.x / mTileSize.y;
    mTileSize -= _viewport_offset_delta(mTileSize.x, ratio);

    const Vector2f minimum {_min_tile_height * ratio, _min_tile_height};
    mTileSize = (glm::max)(minimum, mTileSize);
  }

  mOffset = anchor - (percentage * mTileSize);
}

void Viewport::set_tile_size(const Vector2f& size)
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
  return mTileSize.y > _min_tile_height;
}

auto Viewport::get_scaling_ratio(const Vector2f& tileSize) const -> Vector2f
{
  return mTileSize / tileSize;
}

}  // namespace tactile
