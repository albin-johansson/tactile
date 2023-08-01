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

#include "common/type/math.hpp"
#include "common/type/maybe.hpp"
#include "core/enums/mouse_button.hpp"
#include "core/tiles/tile_pos.hpp"

namespace tactile {

struct ViewportMouseInfo final {
  TilePos tile_pos;           ///< The raw mouse position in tile coordinates.
  Maybe<MouseButton> button;  ///< The active button, if any.
  Float2 raw_pos {};          ///< The raw mouse position.
  Float2 clamped_pos {};  ///< The position clamped to the TL corner of the hovered tile.
  Float2 scaled_pos {};   // TODO document/rename/remove
  bool over_content {};   ///< Indicates whether the mouse is above the viewport contents.
};

struct ViewportLimits final {
  Float2 min_offset {};
  Float2 max_offset {};
};

struct Viewport final {
  Float2 offset {};
  Float2 tile_size {32, 32};
  Maybe<ViewportLimits> limits;
};

/// Component used to track frequently changing aspects of viewports.
/// \note This is mainly used to support features such as viewport centering.
struct DynamicViewportInfo final {
  Maybe<Float2> total_size {};    ///< Size of the viewport itself.
  Maybe<Float2> content_size {};  ///< Size of the rendered content.
};

}  // namespace tactile
