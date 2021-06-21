#include "show_map_viewport.hpp"

#include "imgui.h"
#include "show_grid.hpp"

namespace tactile {
namespace {

inline bool show_grid = true;

}  // namespace

void ShowMapViewport()
{
  static GridState state;
  state.grid_size = {64, 64};

  if (ImGui::Begin("MapViewport"))
  {
    const auto info = GetCanvasInfo();

    FillBackground(info);

    ImGui::InvisibleButton(
        "MapViewportCanvas",
        info.canvas_size,
        ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
    {
      const auto& io = ImGui::GetIO();
      state.scroll_offset.x += io.MouseDelta.x;
      state.scroll_offset.y += io.MouseDelta.y;
    }

    auto* drawList = ImGui::GetWindowDrawList();
    drawList->PushClipRect(info.canvas_screen_pos,
                           info.canvas_offset_pos,
                           true);

    if (show_grid)
    {
      ShowGrid(state, info);
    }

    drawList->PopClipRect();
  }
  ImGui::End();
}

void SetMapViewportGridEnabled(const bool enabled) noexcept
{
  show_grid = enabled;
}

}  // namespace tactile