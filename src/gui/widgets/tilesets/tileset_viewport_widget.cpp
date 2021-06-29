#include "tileset_viewport_widget.hpp"

#include <algorithm>  // min, max

#include "core/tileset/tileset.hpp"
#include "gui/canvas_info.hpp"
#include "gui/get_texture_id.hpp"
#include "gui/show_grid.hpp"
#include "gui/widgets/mouse_tracker.hpp"
#include "imgui.h"
#include "imgui_internal.h"

namespace tactile {
namespace {

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
  state.grid_size = {static_cast<float>(tileset.GetTileWidth()),
                     static_cast<float>(tileset.GetTileHeight())};

  const auto texture = GetTextureID(tileset.GetTexture());
  const auto width = static_cast<float>(tileset.GetWidth());
  const auto height = static_cast<float>(tileset.GetHeight());

  const auto info = GetCanvasInfo();
  FillBackground(info);

  MouseTracker(info, state);
  ClampOffsets(info.canvas_size, width, height);

  auto* drawList = ImGui::GetWindowDrawList();
  drawList->PushClipRect(info.canvas_tl, info.canvas_br, true);

  const auto position = drawList->GetClipRectMin() + state.scroll_offset;
  drawList->AddImage(texture, position, position + ImVec2{width, height});

  ShowGrid(state, info);
  drawList->PopClipRect();
}

}  // namespace tactile
