#include "tileset_view.hpp"

#include <algorithm>  // min, max

#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/rendering/canvas.hpp"
#include "editor/gui/rendering/grid.hpp"
#include "editor/gui/texture_utils.hpp"
#include "editor/gui/widgets/common/rubber_band.hpp"

namespace Tactile {
namespace {

constexpr auto gRubberBandColor = IM_COL32(0, 0x44, 0xCC, 100);

inline GridState gState;

[[nodiscard]] auto TrackScrollOffset(const CanvasInfo& canvas, ImVec2 offset) -> ImVec2
{
  constexpr auto flags = ImGuiButtonFlags_MouseButtonLeft |
                         ImGuiButtonFlags_MouseButtonMiddle |
                         ImGuiButtonFlags_MouseButtonRight;
  ImGui::InvisibleButton("TrackScrollOffset", canvas.size, flags);
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
    const auto& io = ImGui::GetIO();
    offset.x += io.MouseDelta.x;
    offset.y += io.MouseDelta.y;
    return offset;
  }
  else {
    return offset;
  }
}

void UpdateOffsets(const CanvasInfo& canvas, const ImVec2& textureSize)
{
  gState.scroll_offset = TrackScrollOffset(canvas, gState.scroll_offset);

  gState.scroll_offset.x = std::min(0.0f, gState.scroll_offset.x);
  gState.scroll_offset.y = std::min(0.0f, gState.scroll_offset.y);

  gState.scroll_offset.x =
      std::max(-textureSize.x + canvas.size.x, gState.scroll_offset.x);
  gState.scroll_offset.y =
      std::max(-textureSize.y + canvas.size.y, gState.scroll_offset.y);
}

void RenderSelection(const Region& selection, const ImVec2& min, const ImVec2& tileSize)
{
  const auto diff = selection.end - selection.begin;

  const ImVec2 origin{static_cast<float>(selection.begin.GetColumn()) * tileSize.x,
                      static_cast<float>(selection.begin.GetRow()) * tileSize.y};

  const ImVec2 size{static_cast<float>(diff.GetColumn()) * tileSize.x,
                    static_cast<float>(diff.GetRow()) * tileSize.y};

  ImGui::GetWindowDrawList()->AddRectFilled(min + origin,
                                            min + origin + size,
                                            gRubberBandColor);
}

}  // namespace

void UpdateTilesetView(const entt::registry& registry,
                       const entt::entity entity,
                       entt::dispatcher& dispatcher)
{
  const auto& tileset = registry.get<Tileset>(entity);
  const ImVec2 tileSize = {static_cast<float>(tileset.tile_width),
                           static_cast<float>(tileset.tile_height)};
  gState.grid_size = tileSize;

  const auto& texture = registry.get<Texture>(entity);
  const ImVec2 textureSize = {static_cast<float>(texture.width),
                              static_cast<float>(texture.height)};

  const auto canvas = GetCanvasInfo();
  ClearBackground(canvas);
  UpdateOffsets(canvas, textureSize);

  if (const auto selection = RubberBand(gState.scroll_offset, tileSize)) {
    dispatcher.enqueue<SetTilesetSelectionEvent>(*selection);
  }

  auto* drawList = ImGui::GetWindowDrawList();
  drawList->PushClipRect(canvas.tl, canvas.br, true);

  const auto min = drawList->GetClipRectMin() + gState.scroll_offset;
  drawList->AddImage(ToTextureID(texture.id), min, min + textureSize);

  if (const auto& selection = registry.get<TilesetSelection>(entity); selection.region) {
    RenderSelection(*selection.region, min, tileSize);
  }

  ShowGrid(gState, canvas);
  drawList->PopClipRect();
}

}  // namespace Tactile
