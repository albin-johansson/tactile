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

#include <imgui.h>

#include "common/primitives.hpp"
#include "common/type/math.hpp"
#include "core/region.hpp"
#include "core/tile_extent.hpp"
#include "core/viewport.hpp"

namespace tactile::ui {

/// Provides useful information about a canvas for use when rendering.
struct CanvasInfo final {
  ImVec2 top_left {};      ///< Top-left point of the canvas.
  ImVec2 bottom_right {};  ///< Bottom-right point of the canvas.
  ImVec2 size {};          ///< Total size of the canvas.
  Region bounds;           ///< The region of the content that that should be rendered.
  ImVec4 bounds_rect;      ///< Rendering bounds as a rectangle.
  ImVec2 origin_pos {};    ///< Origin screen position.
  ImVec2 graphical_tile_size {};
  ImVec2 logical_tile_size {};
  ImVec2 tile_ratio {};          ///< Graphical tile size divided by logical tile size.
  ImVec2 contents_size {};       ///< Graphical size of the map or tileset.
  int32 tiles_in_viewport_x {};  ///< Amount of visible tiles in the viewport, x-axis.
  int32 tiles_in_viewport_y {};  ///< Amount of visible tiles in the viewport, y-axis.
  float row_count {};            ///< Total amount of rows.
  float col_count {};            ///< Total amount of columns.

  [[nodiscard]] auto contains(const ImVec2& pos) const -> bool;

  [[nodiscard]] auto intersects(const ImVec4& rect) const -> bool;
};

[[nodiscard]] auto create_canvas_info(const Viewport& viewport,
                                      const Float2& logical_tile_size,
                                      const TileExtent& extent) -> CanvasInfo;

}  // namespace tactile::ui
