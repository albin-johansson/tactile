#include "grid.hpp"

namespace Tactile {

void ShowGrid(const GridState& state,
              const CanvasInfo& canvas,
              const uint32 lineColor)
{
  const auto iGridWidth = static_cast<int>(state.grid_size.x);
  const auto iGridHeight = static_cast<int>(state.grid_size.y);
  const auto startX = static_cast<int>(state.scroll_offset.x) % iGridWidth;
  const auto startY = static_cast<int>(state.scroll_offset.y) % iGridHeight;

  auto* drawList = ImGui::GetWindowDrawList();

  for (int x = startX; x < static_cast<int>(canvas.size.x); x += iGridWidth)
  {
    const auto fx = static_cast<float>(x);
    const ImVec2 a{canvas.tl.x + fx, canvas.tl.y};
    const ImVec2 b{canvas.tl.x + fx, canvas.br.y};
    drawList->AddLine(a, b, lineColor);
  }

  for (int y = startY; y < static_cast<int>(canvas.size.y); y += iGridHeight)
  {
    const auto fy = static_cast<float>(y);
    const ImVec2 a{canvas.tl.x, canvas.tl.y + fy};
    const ImVec2 b{canvas.br.x, canvas.tl.y + fy};
    drawList->AddLine(a, b, lineColor);
  }
}

}  // namespace Tactile
