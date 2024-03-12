// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "viewport_cursor_info.hpp"

#include <cmath>  // trunc

#include <imgui_internal.h>

#include "ui/conversions.hpp"
#include "ui/render/renderer.hpp"

namespace tactile::ui {

auto get_viewport_cursor_info(const CanvasInfo& canvas_info) -> ViewportCursorInfo
{
  ViewportCursorInfo cursor;

  const auto mouse = ImGui::GetMousePos();
  cursor.raw_position = mouse - as_imvec2(canvas_info.origin);
  cursor.scaled_position = cursor.raw_position / as_imvec2(canvas_info.ratio);

  const auto index = cursor.raw_position / as_imvec2(canvas_info.grid_size);
  const auto row = std::trunc(index.y);
  const auto col = std::trunc(index.x);

  cursor.is_within_map = index.y >= 0 &&                 //
                         index.x >= 0 &&                 //
                         row < canvas_info.row_count &&  //
                         col < canvas_info.col_count;

  if (cursor.is_within_map) {
    cursor.map_position = {static_cast<int32>(row), static_cast<int32>(col)};
  }

  cursor.clamped_position = {canvas_info.origin.x() + (col * canvas_info.grid_size.x()),
                             canvas_info.origin.y() + (row * canvas_info.grid_size.y())};

  return cursor;
}

}  // namespace tactile::ui
