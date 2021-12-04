#include "tileset_view.hpp"

#include <algorithm>  // min, max

#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/viewport.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/common/rubber_band.hpp"
#include "editor/gui/rendering/canvas.hpp"
#include "editor/gui/rendering/grid.hpp"
#include "editor/gui/texture_utils.hpp"

namespace Tactile {
namespace {

constexpr auto gRubberBandColor = IM_COL32(0, 0x44, 0xCC, 100);

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

void TilesetView::Update(const entt::registry& registry,
                         const entt::entity entity,
                         entt::dispatcher& dispatcher)
{
  const auto region = ImGui::GetContentRegionAvail();
  mWidth = region.x;
  mHeight = region.y;

  const auto& tileset = registry.get<Tileset>(entity);
  const auto& viewport = registry.get<Viewport>(entity);

  const ImVec2 tileSize = {static_cast<float>(tileset.tile_width),
                           static_cast<float>(tileset.tile_height)};

  const auto& texture = registry.get<Texture>(entity);
  const ImVec2 textureSize = {static_cast<float>(texture.width),
                              static_cast<float>(texture.height)};

  const auto canvas = GetCanvasInfo();
  ClearBackground(canvas);

  const auto offset = ImVec2{viewport.x_offset, viewport.y_offset};
  const auto scroll = TrackScrollOffset(canvas, offset);

  if (const auto selection = RubberBand(scroll, tileSize)) {
    dispatcher.enqueue<SetTilesetSelectionEvent>(*selection);
  }

  auto* drawList = ImGui::GetWindowDrawList();
  drawList->PushClipRect(canvas.tl, canvas.br, true);

  const auto min = drawList->GetClipRectMin() + offset;
  drawList->AddImage(ToTextureID(texture.id), min, min + textureSize);

  if (const auto& selection = registry.get<TilesetSelection>(entity); selection.region) {
    RenderSelection(*selection.region, min, tileSize);
  }

  RenderGrid(offset, {viewport.tile_width, viewport.tile_height}, canvas);
  drawList->PopClipRect();
}

auto TilesetView::GetWidth() const -> Maybe<float>
{
  return mWidth;
}

auto TilesetView::GetHeight() const -> Maybe<float>
{
  return mHeight;
}

}  // namespace Tactile
