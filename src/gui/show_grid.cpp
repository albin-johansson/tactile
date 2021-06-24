#include "show_grid.hpp"

namespace tactile {
namespace {

inline constexpr auto border_color = IM_COL32(255, 255, 255, 100);
inline constexpr auto background_color = IM_COL32(50, 50, 50, 255);
inline constexpr auto line_color = IM_COL32(200, 200, 200, 40);

}  // namespace

auto GetCanvasInfo() -> CanvasInfo
{
  CanvasInfo info;

  info.canvas_size = ImGui::GetContentRegionAvail();
  info.canvas_tl = ImGui::GetCursorScreenPos();
  info.canvas_br = {info.canvas_tl.x + info.canvas_size.x,
                    info.canvas_tl.y + info.canvas_size.y};

  return info;
}

void FillBackground(const CanvasInfo& info)
{
  auto* drawList = ImGui::GetWindowDrawList();
  drawList->AddRectFilled(info.canvas_tl, info.canvas_br, background_color);
  drawList->AddRect(info.canvas_tl, info.canvas_br, border_color);
}

void ShowGrid(const GridState& state, const CanvasInfo& info)
{
  const auto iGridWidth = static_cast<int>(state.grid_size.x);
  const auto iGridHeight = static_cast<int>(state.grid_size.y);
  const auto startX = static_cast<int>(state.scroll_offset.x) % iGridWidth;
  const auto startY = static_cast<int>(state.scroll_offset.y) % iGridHeight;

  auto* drawList = ImGui::GetWindowDrawList();

  for (int x = startX; x < static_cast<int>(info.canvas_size.x);
       x += iGridWidth)
  {
    const auto fx = static_cast<float>(x);
    const ImVec2 a{info.canvas_tl.x + fx, info.canvas_tl.y};
    const ImVec2 b{info.canvas_tl.x + fx, info.canvas_br.y};
    drawList->AddLine(a, b, line_color);
  }

  for (int y = startY; y < static_cast<int>(info.canvas_size.y);
       y += iGridHeight)
  {
    const auto fy = static_cast<float>(y);
    const ImVec2 a{info.canvas_tl.x, info.canvas_tl.y + fy};
    const ImVec2 b{info.canvas_br.x, info.canvas_tl.y + fy};
    drawList->AddLine(a, b, line_color);
  }
}

}  // namespace tactile
