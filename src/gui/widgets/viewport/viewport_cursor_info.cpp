#include "viewport_cursor_info.hpp"

#include <imgui_internal.h>

#include <cmath>  // trunc

namespace Tactile {

[[nodiscard]] auto GetViewportCursorInfo(const ImVec2& mapOrigin,
                                         const ImVec2& gridSize,
                                         const float nRows,
                                         const float nCols) -> ViewportCursorInfo
{
  ViewportCursorInfo info;

  const auto mouse = ImGui::GetMousePos();
  const auto item = ImGui::GetItemRectMin();
  const auto diff = mouse - mapOrigin;

  const auto xRow = diff.y / gridSize.y;
  const auto xCol = diff.x / gridSize.x;
  const auto row = std::trunc(xRow);
  const auto col = std::trunc(xCol);

  info.is_within_map = xRow >= 0 && xCol >= 0 && row < nRows && col < nCols;

  if (info.is_within_map)
  {
    info.map_position = {AsRow(row), AsColumn(col)};
  }

  info.raw_position = {mapOrigin.x + (col * gridSize.x),
                       mapOrigin.y + (row * gridSize.y)};

  return info;
}

}  // namespace Tactile
