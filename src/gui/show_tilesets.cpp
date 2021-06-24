#include "show_tilesets.hpp"

#include <IconsFontAwesome5.h>

#include "imgui.h"
#include "show_grid.hpp"
#include "widgets/button_ex.hpp"

namespace tactile {

void ShowTilesets()
{
  static GridState state;
  state.grid_size = {32, 32};

  if (ImGui::Begin("Tilesets"))
  {
    if (ButtonEx(ICON_FA_PLUS_CIRCLE, "Create tileset."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_MINUS_CIRCLE, "Remove tileset."))
    {}

    const auto info = GetCanvasInfo();

    FillBackground(info);

    ImGui::InvisibleButton(
        "TilesetCanvas",
        info.canvas_size,
        ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
    {
      const auto& io = ImGui::GetIO();
      state.scroll_offset.x += io.MouseDelta.x;
      state.scroll_offset.y += io.MouseDelta.y;
    }

    auto* drawList = ImGui::GetWindowDrawList();
    drawList->PushClipRect(info.canvas_tl, info.canvas_br, true);

    ShowGrid(state, info);

    drawList->PopClipRect();
  }

  ImGui::End();
}

}  // namespace tactile
