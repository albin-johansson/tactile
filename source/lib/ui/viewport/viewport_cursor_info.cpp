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

#include "viewport_cursor_info.hpp"

#include <cmath>  // trunc

#include <imgui_internal.h>

namespace tactile::ui {

auto get_viewport_cursor_info(const CanvasInfo& canvas) -> ViewportCursorInfo
{
  ViewportCursorInfo cursor;

  cursor.raw_position = ImGui::GetMousePos() - canvas.origin_pos;
  cursor.scaled_position = cursor.raw_position / canvas.tile_ratio;

  const auto index = cursor.raw_position / canvas.graphical_tile_size;
  const auto row = std::trunc(index.y);
  const auto col = std::trunc(index.x);

  cursor.is_within_map = index.y >= 0 &&            //
                         index.x >= 0 &&            //
                         row < canvas.row_count &&  //
                         col < canvas.col_count;

  if (cursor.is_within_map) {
    cursor.map_position = {static_cast<int32>(row), static_cast<int32>(col)};
  }

  cursor.clamped_position = {canvas.origin_pos.x + (col * canvas.graphical_tile_size.x),
                             canvas.origin_pos.y + (row * canvas.graphical_tile_size.y)};

  return cursor;
}

}  // namespace tactile::ui
