#include "tileset_widget.hpp"

#include <IconsFontAwesome5.h>

#include "gui/show_grid.hpp"
#include "gui/widgets/button_ex.hpp"
#include "imgui.h"

namespace tactile {
namespace {

inline bool is_visible = true;

}  // namespace

void UpdateTilesetWidget()
{
  static GridState state;
  state.grid_size = {32, 32};

  if (!is_visible)
  {
    return;
  }

  if (ImGui::Begin("Tilesets", &is_visible, ImGuiWindowFlags_NoCollapse))
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

void SetTilesetWidgetVisible(const bool visible) noexcept
{
  is_visible = visible;
}

auto IsTilesetWidgetVisible() noexcept -> bool
{
  return is_visible;
}

}  // namespace tactile
