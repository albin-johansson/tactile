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
#include "io/persistence/preferences.hpp"

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

  const ImVec2 origin{static_cast<float>(selection.begin.col()) * tileSize.x,
                      static_cast<float>(selection.begin.row()) * tileSize.y};

  const ImVec2 size{static_cast<float>(diff.col()) * tileSize.x,
                    static_cast<float>(diff.row()) * tileSize.y};

  ImGui::GetWindowDrawList()->AddRectFilled(min + origin,
                                            min + origin + size,
                                            gRubberBandColor);
}

void RenderTilesetImage(graphics_ctx& graphics,
                        const comp::texture& texture,
                        const ImVec2& position)
{
  const ImVec2 size = {static_cast<float>(texture.width),
                       static_cast<float>(texture.height)};
  graphics.render_image(texture.id, position, size);
}

}  // namespace

void TilesetView::Update(const entt::registry& registry,
                         const entt::entity entity,
                         entt::dispatcher& dispatcher)
{
  const auto& tileset = registry.get<comp::tileset>(entity);
  const auto& viewport = registry.get<Viewport>(entity);

  const auto region = ImGui::GetContentRegionAvail();
  mWidth = region.x;
  mHeight = region.y;

  const auto info = GetRenderInfo(viewport, tileset);
  graphics_ctx graphics{info};

  graphics.set_draw_color(get_preferences().viewport_bg());
  graphics.clear();

  const auto offset = ImVec2{viewport.x_offset, viewport.y_offset};
  const auto scroll = TrackScrollOffset(info.canvas_br - info.canvas_tl, offset);

  const ImVec2 tileSize = {static_cast<float>(tileset.tile_width),
                           static_cast<float>(tileset.tile_height)};

  if (const auto selection = rubber_band(scroll, tileSize)) {
    dispatcher.enqueue<set_tileset_selection_event>(*selection);
  }

  graphics.push_clip();

  const auto position = ImGui::GetWindowDrawList()->GetClipRectMin() + offset;
  RenderTilesetImage(graphics, registry.get<comp::texture>(entity), position);

  if (const auto& selection = registry.get<comp::tileset_selection>(entity);
      selection.region) {
    RenderSelection(*selection.region, position, tileSize);
  }

  graphics.set_line_thickness(1);
  graphics.set_draw_color(gGridColor);
  graphics.render_translated_grid();

  graphics.pop_clip();
}

auto TilesetView::GetWidth() const -> maybe<float>
{
  return mWidth;
}

auto TilesetView::GetHeight() const -> maybe<float>
{
  return mHeight;
}

}  // namespace tactile
