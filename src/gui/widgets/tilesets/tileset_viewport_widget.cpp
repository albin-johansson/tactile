#include "tileset_viewport_widget.hpp"

#include <algorithm>  // min, max
#include <cmath>      // round, fmod

#include "core/events/set_tileset_selection_event.hpp"
#include "core/tileset/tileset.hpp"
#include "gui/canvas_info.hpp"
#include "gui/get_texture_id.hpp"
#include "gui/show_grid.hpp"
#include "gui/widgets/mouse_tracker.hpp"
#include "imgui.h"
#include "imgui_internal.h"

namespace Tactile {
namespace {

inline constexpr auto rubber_band_color = IM_COL32(0, 0x44, 0xCC, 100);

inline GridState state;

void ClampOffsets(const ImVec2 canvasSize,
                  const float width,
                  const float height)
{
  state.scroll_offset.x = std::min(0.0f, state.scroll_offset.x);
  state.scroll_offset.y = std::min(0.0f, state.scroll_offset.y);

  state.scroll_offset.x =
      std::max(-width + canvasSize.x, state.scroll_offset.x);
  state.scroll_offset.y =
      std::max(-height + canvasSize.y, state.scroll_offset.y);
}

}  // namespace

void TilesetViewportWidget(const Tileset& tileset, entt::dispatcher& dispatcher)
{
  const auto tileWidth = static_cast<float>(tileset.GetTileWidth());
  const auto tileHeight = static_cast<float>(tileset.GetTileHeight());

  state.grid_size = {tileWidth, tileHeight};

  const auto texture = GetTextureID(tileset.GetTexture());
  const auto width = static_cast<float>(tileset.GetWidth());
  const auto height = static_cast<float>(tileset.GetHeight());

  const auto info = GetCanvasInfo();
  FillBackground(info);

  MouseTracker(info, state);

  ClampOffsets(info.canvas_size, width, height);

  auto* drawList = ImGui::GetWindowDrawList();

  const auto toRow = [=](const float y) {
    return row_t{static_cast<row_t::value_type>(std::round(y / tileHeight))};
  };

  const auto toColumn = [=](const float x) {
    return col_t{static_cast<col_t::value_type>(std::round(x / tileWidth))};
  };

  if (ImGui::IsItemActive())
  {
    const auto button = ImGuiMouseButton_Left;
    const auto clicked = ImGui::IsMouseClicked(button);
    const auto dragging = ImGui::IsMouseDragging(button);
    if (clicked || dragging)
    {
      const auto mousePos = ImGui::GetMousePos();
      const auto itemPos = ImGui::GetItemRectMin();
      const auto delta = dragging ? ImGui::GetMouseDragDelta(button) : ImVec2{};

      const auto relativeToViewport = mousePos - itemPos - delta;
      const auto effective = relativeToViewport - state.scroll_offset;

      const auto startX = effective.x - std::fmod(effective.x, tileWidth);
      const auto startY = effective.y - std::fmod(effective.y, tileHeight);

      TilesetSelection selection;
      selection.top_left = {toRow(startY), toColumn(startX)};

      const MapPosition offset{toRow(delta.y) + 1_row,
                               toColumn(delta.x) + 1_col};
      selection.bottom_right = selection.top_left + offset;

      dispatcher.enqueue<SetTilesetSelectionEvent>(selection);
    }
  }

  drawList->PushClipRect(info.canvas_tl, info.canvas_br, true);

  const auto position = drawList->GetClipRectMin() + state.scroll_offset;
  drawList->AddImage(texture, position, position + ImVec2{width, height});

  if (const auto& selection = tileset.GetSelection())
  {
    const auto tl = selection->top_left;
    const auto br = selection->bottom_right;
    const auto diff = br - tl;

    const ImVec2 size = {static_cast<float>(diff.GetColumn()) * tileWidth,
                         static_cast<float>(diff.GetRow()) * tileHeight};

    const ImVec2 origin = {static_cast<float>(tl.GetColumn()) * tileWidth,
                           static_cast<float>(tl.GetRow()) * tileHeight};

    drawList->AddRectFilled(position + origin,
                            position + origin + size,
                            rubber_band_color);
  }

  ShowGrid(state, info);
  drawList->PopClipRect();
}

}  // namespace Tactile
