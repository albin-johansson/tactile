// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "viewport.hpp"

#include <algorithm>  // max
#include <cmath>      // round

#include "tactile/core/debug/assert.hpp"

namespace tactile {
namespace {

constexpr float kViewportMinTileHeight = 4;

[[nodiscard]] auto viewport_offset_delta(const float tile_width, const float ratio)
    -> Float2
{
  const auto dx = std::round(std::max(2.0f, tile_width * 0.05f));
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
    mOffset = max(mLimits->min_offset, mOffset);
    mOffset = min(mLimits->max_offset, mOffset);
  }
}

void Viewport::pan_left()
{
  mOffset[0] += mTileSize[0];
}

void Viewport::pan_right()
{
  mOffset[0] -= mTileSize[0];
}

void Viewport::pan_up()
{
  mOffset[1] += mTileSize[1];
}

void Viewport::pan_down()
{
  mOffset[1] -= mTileSize[1];
}

void Viewport::zoom_in(const Float2& anchor)
{
  // Percentages of area to the left of and above the cursor
  const auto percentage = (anchor - mOffset) / mTileSize;

  mTileSize += viewport_offset_delta(mTileSize.x(), mTileSize.x() / mTileSize.y());
  mOffset = anchor - (percentage * mTileSize);
}

void Viewport::zoom_out(const Float2& anchor)
{
  // Percentages of area to the left of and above the cursor
  const auto percentage = (anchor - mOffset) / mTileSize;

  {
    const auto ratio = mTileSize.x() / mTileSize.y();
    mTileSize -= viewport_offset_delta(mTileSize.x(), ratio);

    const Float2 minimum {kViewportMinTileHeight * ratio, kViewportMinTileHeight};
    mTileSize = max(minimum, mTileSize);
  }

  mOffset = anchor - (percentage * mTileSize);
}

void Viewport::set_tile_size(const Float2& size)
{
  TACTILE_ASSERT(size.x() > 0);
  TACTILE_ASSERT(size.y() > 0);
  mTileSize = size;
}

void Viewport::set_limits(const ViewportLimits& limits)
{
  mLimits = limits;
}

auto Viewport::can_zoom_out() const -> bool
{
  return mTileSize.y() > kViewportMinTileHeight;
}

auto Viewport::scaling_ratio(const Float2& tileSize) const -> Float2
{
  return mTileSize / tileSize;
}

}  // namespace tactile
