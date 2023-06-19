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

#pragma once

#include "common/enum/mouse_button.hpp"
#include "common/tile_pos.hpp"
#include "common/type/math.hpp"
#include "common/type/maybe.hpp"

namespace tactile {

struct ViewportLimits final {
  Float2 min_offset {};
  Float2 max_offset {};
};

struct ViewportMouseInfo final {
  TilePos tile_pos;           ///< The raw mouse position in tile coordinates.
  Maybe<MouseButton> button;  ///< The active button, if any.
  Float2 raw_pos {};          ///< The raw mouse position.
  Float2 clamped_pos {};  ///< The position clamped to the TL corner of the hovered tile.
  Float2 scaled_pos {};   // TODO document/rename/remove
  bool in_viewport {};    ///< Indicates whether the mouse is within the viewport.
};

struct Viewport final {
  Float2 offset {};
  Float2 tile_size {32, 32};
  Maybe<ViewportLimits> limits;

  [[nodiscard]] auto scaling_ratio(const Float2& logical_tile_size) const -> Float2;

  [[nodiscard]] auto can_zoom_out() const -> bool;
};

/// Component used to track frequently changing aspects of viewports.
/// \note This is mainly used to support features such as viewport centering.
struct DynamicViewportInfo final {
  Maybe<Float2> total_size {};    ///< Size of the viewport itself.
  Maybe<Float2> content_size {};  ///< Size of the rendered content.
};

}  // namespace tactile
