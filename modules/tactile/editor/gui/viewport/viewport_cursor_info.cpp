#include "viewport_cursor_info.hpp"

#include <cmath>  // trunc

#include <imgui_internal.h>

#include "editor/gui/rendering/render_info.hpp"

namespace Tactile {

auto GetViewportCursorInfo(const RenderInfo& info) -> ViewportCursorInfo
{
  ViewportCursorInfo cursor;

  const auto mouse = ImGui::GetMousePos();
  cursor.raw_position = mouse - info.origin;

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

}  // namespace Tactile
