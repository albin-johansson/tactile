#include "show_map_viewport.hpp"

#include "core/map_document.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "show_grid.hpp"

namespace tactile {
namespace {

inline constexpr auto border_color = IM_COL32(0x10, 0x10, 0x10, 0xFF);

inline bool show_grid = true;
inline bool center_viewport = false;
inline GridState state;

inline constexpr auto viewport_button_flags =
    ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight;

void DrawMap(ImDrawList* drawList,
             const ImVec2 offset,
             const MapDocument& document)
{
  const auto nRows = document.GetRowCount();
  const auto nCols = document.GetColumnCount();
  for (auto row = 0; row < nRows; ++row)
  {
    for (auto col = 0; col < nCols; ++col)
    {
      const auto x = offset.x + (state.grid_size.x * static_cast<float>(col));
      const auto y = offset.y + (state.grid_size.y * static_cast<float>(row));

      drawList->AddRect({x, y},
                        {x + state.grid_size.x, y + state.grid_size.y},
                        border_color);
    }
  }

  const auto width = static_cast<float>(nCols) * state.grid_size.x;
  const auto height = static_cast<float>(nRows) * state.grid_size.y;

  drawList->AddLine(offset, {offset.x + width, offset.y}, border_color);
  drawList->AddLine(offset, {offset.x, offset.y + height}, border_color);

  drawList->AddLine({offset.x, offset.y + height},
                    {offset.x + width, offset.y + height},
                    border_color);
  drawList->AddLine({offset.x + width, offset.y},
                    {offset.x + width, offset.y + height},
                    border_color);
}

void MouseTracker(const CanvasInfo& info)
{
  ImGui::InvisibleButton("MapViewportCanvas",
                         info.canvas_size,
                         viewport_button_flags);
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
  {
    const auto& io = ImGui::GetIO();
    state.scroll_offset.x += io.MouseDelta.x;
    state.scroll_offset.y += io.MouseDelta.y;
  }
}

}  // namespace

void ShowMapViewport(const MapDocument* document)
{
  state.grid_size = {64, 64};

  if (ImGui::Begin("MapViewport"))
  {
    auto* drawList = ImGui::GetWindowDrawList();
    const auto info = GetCanvasInfo();

    FillBackground(info);
    MouseTracker(info);

    drawList->PushClipRect(info.canvas_tl, info.canvas_br, true);

    if (center_viewport && document)
    {
      const auto width = document->GetColumnCount().get() * state.grid_size.x;
      const auto height = document->GetRowCount().get() * state.grid_size.y;

      state.scroll_offset.x = (info.canvas_size.x - width) / 2.0f;
      state.scroll_offset.y = (info.canvas_size.y - height) / 2.0f;

      center_viewport = false;
    }

    if (show_grid)
    {
      ShowGrid(state, info);
    }

    if (document)
    {
      const auto offset = info.canvas_tl + state.scroll_offset;
      DrawMap(drawList, offset, *document);
    }

    drawList->PopClipRect();
  }
  ImGui::End();
}

void CenterMapViewport()
{
  center_viewport = true;
}

void SetMapViewportGridEnabled(const bool enabled) noexcept
{
  show_grid = enabled;
}

}  // namespace tactile