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

#include "viewport_cursor_info.hpp"

#include <cmath>  // trunc

#include <imgui_internal.h>

#include "editor/gui/rendering/render_info.hpp"

namespace tactile {

auto GetViewportCursorInfo(const render_info& info) -> ViewportCursorInfo
{
  ViewportCursorInfo cursor;

  const auto mouse = ImGui::GetMousePos();
  cursor.raw_position = mouse - info.origin;
  cursor.scaled_position = cursor.raw_position / info.ratio;

  const auto index = cursor.raw_position / info.grid_size;
  const auto row = std::trunc(index.y);
  const auto col = std::trunc(index.x);

  cursor.is_within_map =
      index.y >= 0 && index.x >= 0 && row < info.row_count && col < info.col_count;

  if (cursor.is_within_map) {
    cursor.map_position = {static_cast<int32>(row), static_cast<int32>(col)};
  }

  cursor.clamped_position = {info.origin.x + (col * info.grid_size.x),
                             info.origin.y + (row * info.grid_size.y)};

  return cursor;
}

}  // namespace tactile
