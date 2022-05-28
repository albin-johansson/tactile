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

#include "rubber_band.hpp"

#include <algorithm>  // max, min
#include <cmath>      // fmod

#include <centurion/math.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace tactile::ui {

auto rubber_band(const ImVec2& scrollOffset, const ImVec2& tileSize) -> Maybe<Region>
{
  const auto toRow = [=](const float y) {
    const auto mod = y - std::fmod(y, tileSize.y);
    return static_cast<int32>(mod / tileSize.y);
  };

  const auto toColumn = [=](const float x) {
    const auto mod = x - std::fmod(x, tileSize.x);
    return static_cast<int32>(mod / tileSize.x);
  };

  if (ImGui::IsItemActive()) {
    constexpr auto button = ImGuiMouseButton_Left;
    const auto dragging = ImGui::IsMouseDragging(button);
    if (ImGui::IsMouseClicked(button) || dragging) {
      const auto mousePos = ImGui::GetMousePos();
      const auto itemPos = ImGui::GetItemRectMin();
      const auto delta = dragging ? ImGui::GetMouseDragDelta(button) : ImVec2{};

      const auto current = mousePos - itemPos;
      const auto origin = current - delta;
      const auto offsetOrigin = origin - scrollOffset;

      // p0 is the origin position, p1 is the current mouse position
      const auto p0 = offsetOrigin;
      const auto p1 = offsetOrigin + delta;

      // Determine coordinates for top-left and bottom-right points
      const auto x1 = (std::min)(p0.x, p1.x);
      const auto y1 = (std::min)(p0.y, p1.y);
      const auto x2 = (std::max)(p0.x, p1.x);
      const auto y2 = (std::max)(p0.y, p1.y);

      // Convert coordinates to row/column indices
      const auto r1 = toRow(y1);
      const auto r2 = toRow(y2);
      const auto c1 = toColumn(x1);
      const auto c2 = toColumn(x2);

      // Ensure that the selection width and height are non-zero
      const auto w = (std::max)(1, c2 - c1);
      const auto h = (std::max)(1, r2 - r1);

      // Calculate width/height offsets for single tile selections
      const auto ww = (c1 != c2) ? 1 : 0;
      const auto hh = (r1 != r2) ? 1 : 0;

      cen::frect rect;
      rect.set_x(static_cast<float>(c1) * tileSize.x);
      rect.set_y(static_cast<float>(r1) * tileSize.y);
      rect.set_width(static_cast<float>(w + ww) * tileSize.x);
      rect.set_height(static_cast<float>(h + hh) * tileSize.y);

      Region selection;
      selection.begin = {toRow(rect.y()), toColumn(rect.x())};

      const TilePos offset{toRow(rect.max_y() - rect.y()),
                           toColumn(rect.max_x() - rect.x())};
      selection.end = selection.begin + offset;

      return selection;
    }
  }

  return nothing;
}

}  // namespace tactile::ui