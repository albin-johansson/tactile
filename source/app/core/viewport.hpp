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

namespace tactile::core {

struct ViewportLimits final
{
  Vector2f min_offset{};
  Vector2f max_offset{};
};

class Viewport final
{
 public:
  void reset_limits();

  void reset_zoom();

  void offset(const Vector2f& delta);

  void pan_left();

  void pan_right();

  void pan_up();

  void pan_down();

  void zoom_in(const Vector2f& anchor);

  void zoom_out(const Vector2f& anchor);

  void set_cell_size(const Vector2f& size);

  void set_limits(const ViewportLimits& limits);

  [[nodiscard]] auto can_zoom_out() const -> bool;

  [[nodiscard]] auto get_scaling_ratio(const Vector2f& tileSize) const -> Vector2f;

  [[nodiscard]] auto get_offset() const noexcept -> const Vector2f& { return mOffset; }

  [[nodiscard]] auto get_cell_size() const noexcept -> const Vector2f&
  {
    return mCellSize;
  }

  [[nodiscard]] auto get_limits() const noexcept -> const Maybe<ViewportLimits>&
  {
    return mLimits;
  }

 private:
  Vector2f              mOffset{};
  Vector2f              mCellSize{32, 32};
  Maybe<ViewportLimits> mLimits;
};

}  // namespace tactile::core
