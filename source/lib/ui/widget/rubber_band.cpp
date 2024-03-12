// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "rubber_band.hpp"

#include <algorithm>  // min, max
#include <cmath>      // fmod

#include <centurion/math.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace tactile::ui {

auto ui_rubber_band(const ImVec2& scroll_offset, const ImVec2& tile_size) -> Maybe<Region>
{
  const auto to_row = [=](const float y) {
    const auto mod = y - std::fmod(y, tile_size.y);
    return static_cast<int32>(mod / tile_size.y);
  };

  const auto to_column = [=](const float x) {
    const auto mod = x - std::fmod(x, tile_size.x);
    return static_cast<int32>(mod / tile_size.x);
  };

  if (ImGui::IsItemActive()) {
    constexpr auto button = ImGuiMouseButton_Left;
    const auto dragging = ImGui::IsMouseDragging(button);
    if (ImGui::IsMouseClicked(button) || dragging) {
      const auto mouse_pos = ImGui::GetMousePos();
      const auto item_pos = ImGui::GetItemRectMin();
      const auto delta = dragging ? ImGui::GetMouseDragDelta(button) : ImVec2 {};

      const auto current = mouse_pos - item_pos;
      const auto origin = current - delta;
      const auto offset_origin = origin - scroll_offset;

      // p0 is the origin position, p1 is the current mouse position
      const auto p0 = offset_origin;
      const auto p1 = offset_origin + delta;

      // Determine coordinates for top-left and bottom-right points
      const auto x1 = std::min(p0.x, p1.x);
      const auto y1 = std::min(p0.y, p1.y);
      const auto x2 = std::max(p0.x, p1.x);
      const auto y2 = std::max(p0.y, p1.y);

      // Convert coordinates to row/column indices
      const auto r1 = to_row(y1);
      const auto r2 = to_row(y2);
      const auto c1 = to_column(x1);
      const auto c2 = to_column(x2);

      // Ensure that the selection width and height are non-zero
      const auto w = std::max(1, c2 - c1);
      const auto h = std::max(1, r2 - r1);

      // Calculate width/height offsets for single tile selections
      const auto ww = (c1 != c2) ? 1 : 0;
      const auto hh = (r1 != r2) ? 1 : 0;

      cen::frect rect;
      rect.set_x(static_cast<float>(c1) * tile_size.x);
      rect.set_y(static_cast<float>(r1) * tile_size.y);
      rect.set_width(static_cast<float>(w + ww) * tile_size.x);
      rect.set_height(static_cast<float>(h + hh) * tile_size.y);

      Region selection;
      selection.begin = {to_row(rect.y()), to_column(rect.x())};

      const TilePos offset {to_row(rect.max_y() - rect.y()),
                            to_column(rect.max_x() - rect.x())};
      selection.end = selection.begin + offset;

      return selection;
    }
  }

  return nothing;
}

}  // namespace tactile::ui
