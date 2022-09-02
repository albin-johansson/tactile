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

#pragma once

#include "core/common/math.hpp"
#include "core/common/maybe.hpp"

namespace tactile {

struct ViewportLimits final
{
  float2 min_offset {};
  float2 max_offset {};
};

class Viewport final
{
 public:
  void reset_limits();

  void offset(const float2& delta);

  void pan_left();

  void pan_right();

  void pan_up();

  void pan_down();

  void zoom_in(const float2& anchor);

  void zoom_out(const float2& anchor);

  void set_tile_size(const float2& size);

  void set_limits(const ViewportLimits& limits);

  [[nodiscard]] auto can_zoom_out() const -> bool;

  [[nodiscard]] auto get_scaling_ratio(const float2& tileSize) const -> float2;

  [[nodiscard]] auto get_offset() const noexcept -> const float2&
  {
    return mOffset;
  }

  [[nodiscard]] auto get_tile_size() const noexcept -> const float2&
  {
    return mTileSize;
  }

  [[nodiscard]] auto get_limits() const noexcept -> const Maybe<ViewportLimits>&
  {
    return mLimits;
  }

 private:
  float2 mOffset {};
  float2 mTileSize {32, 32};
  Maybe<ViewportLimits> mLimits;
};

}  // namespace tactile
