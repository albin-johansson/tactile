#include "tileset_view.hpp"

#include <centurion.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/viewport.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/common/rubber_band.hpp"
#include "editor/gui/rendering/graphics.hpp"
#include "editor/gui/rendering/render_info.hpp"
#include "io/preferences.hpp"

namespace tactile {
namespace {

constexpr auto gRubberBandColor = IM_COL32(0, 0x44, 0xCC, 100);
constexpr cen::color gGridColor{200, 200, 200, 40};

[[nodiscard]] auto TrackScrollOffset(const ImVec2& canvasSize, ImVec2 offset) -> ImVec2
{
  constexpr auto flags = ImGuiButtonFlags_MouseButtonLeft |
                         ImGuiButtonFlags_MouseButtonMiddle |
                         ImGuiButtonFlags_MouseButtonRight;
  ImGui::InvisibleButton("TrackScrollOffset", canvasSize, flags);
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

void RenderTilesetImage(Graphics& graphics,
                        const Texture& texture,
                        const ImVec2& position)
{
  const ImVec2 size = {static_cast<float>(texture.width),
                       static_cast<float>(texture.height)};
  graphics.RenderImage(texture.id, position, size);
}

}  // namespace

void TilesetView::Update(const entt::registry& registry,
                         const entt::entity entity,
                         entt::dispatcher& dispatcher)
{
  const auto& tileset = registry.get<Tileset>(entity);
  const auto& viewport = registry.get<Viewport>(entity);

  const auto region = ImGui::GetContentRegionAvail();
  mWidth = region.x;
  mHeight = region.y;

  const auto info = GetRenderInfo(viewport, tileset);
  Graphics graphics{info};

  graphics.SetDrawColor(Prefs::GetViewportBackground());
  graphics.Clear();

  const auto offset = ImVec2{viewport.x_offset, viewport.y_offset};
  const auto scroll = TrackScrollOffset(info.canvas_br - info.canvas_tl, offset);

  const ImVec2 tileSize = {static_cast<float>(tileset.tile_width),
                           static_cast<float>(tileset.tile_height)};

  if (const auto selection = RubberBand(scroll, tileSize)) {
    dispatcher.enqueue<SetTilesetSelectionEvent>(*selection);
  }

  graphics.PushClip();

  const auto position = ImGui::GetWindowDrawList()->GetClipRectMin() + offset;
  RenderTilesetImage(graphics, registry.get<Texture>(entity), position);

  if (const auto& selection = registry.get<TilesetSelection>(entity); selection.region) {
    RenderSelection(*selection.region, position, tileSize);
  }

  graphics.SetLineThickness(1);
  graphics.SetDrawColor(gGridColor);
  graphics.RenderTranslatedGrid();

  graphics.PopClip();
}

auto TilesetView::GetWidth() const -> Maybe<float>
{
  return mWidth;
}

auto TilesetView::GetHeight() const -> Maybe<float>
{
  return mHeight;
}

}  // namespace tactile
