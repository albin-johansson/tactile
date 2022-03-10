/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tileset_view.hpp"

#include <centurion.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/texture.hpp"
#include "core/components/tiles.hpp"
#include "core/viewport.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/common/rubber_band.hpp"
#include "editor/gui/rendering/graphics.hpp"
#include "editor/gui/rendering/render_info.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile {
namespace {

constexpr auto _rubber_band_color = IM_COL32(0, 0x44, 0xCC, 100);
constexpr cen::color _grid_color{200, 200, 200, 40};

void _update_viewport_offset(const entt::entity viewportEntity,
                             entt::dispatcher& dispatcher,
                             const ImVec2& viewportSize)
{
  ImGui::InvisibleButton("TilesetViewInvisibleButton",
                         viewportSize,
                         ImGuiButtonFlags_MouseButtonLeft |
                             ImGuiButtonFlags_MouseButtonMiddle |
                             ImGuiButtonFlags_MouseButtonRight);
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
    const auto& io = ImGui::GetIO();
    dispatcher.enqueue<OffsetBoundViewportEvent>(viewportEntity,
                                                 io.MouseDelta.x,
                                                 io.MouseDelta.y,
                                                 viewportSize.x,
                                                 viewportSize.y);
  }
}

void _render_selection(const Region& selection, const ImVec2& min, const ImVec2& tileSize)
{
  const auto diff = selection.end - selection.begin;

  const ImVec2 origin{static_cast<float>(selection.begin.col()) * tileSize.x,
                      static_cast<float>(selection.begin.row()) * tileSize.y};

  const ImVec2 size{static_cast<float>(diff.col()) * tileSize.x,
                    static_cast<float>(diff.row()) * tileSize.y};

  ImGui::GetWindowDrawList()->AddRectFilled(min + origin,
                                            min + origin + size,
                                            _rubber_band_color);
}

void _render_tileset_image(GraphicsCtx& graphics,
                           const comp::Texture& texture,
                           const ImVec2& position)
{
  const ImVec2 size = {static_cast<float>(texture.width),
                       static_cast<float>(texture.height)};
  graphics.render_image(texture.id, position, size);
}

}  // namespace

void TilesetView::update(const entt::registry& registry,
                         const entt::entity entity,
                         entt::dispatcher& dispatcher)
{
  const auto& tileset = registry.get<comp::Tileset>(entity);
  const auto& viewport = registry.get<Viewport>(entity);

  const auto region = ImGui::GetContentRegionAvail();
  mWidth = region.x;
  mHeight = region.y;

  const auto info = get_render_info(viewport, tileset);
  _update_viewport_offset(entity, dispatcher, info.canvas_br - info.canvas_tl);

  GraphicsCtx graphics{info};
  graphics.set_draw_color(get_preferences().viewport_bg());
  graphics.clear();

  const ImVec2 offset{viewport.x_offset, viewport.y_offset};
  const ImVec2 tileSize = {static_cast<float>(tileset.tile_width),
                           static_cast<float>(tileset.tile_height)};

  if (const auto selection = rubber_band(offset, tileSize)) {
    dispatcher.enqueue<SetTilesetSelectionEvent>(*selection);
  }

  graphics.push_clip();

  const auto position = ImGui::GetWindowDrawList()->GetClipRectMin() + offset;
  _render_tileset_image(graphics, registry.get<comp::Texture>(entity), position);

  if (const auto& selection = registry.get<comp::TilesetSelection>(entity);
      selection.region) {
    _render_selection(*selection.region, position, tileSize);
  }

  graphics.set_line_thickness(1);
  graphics.set_draw_color(_grid_color);
  graphics.render_translated_grid();

  graphics.pop_clip();
}

auto TilesetView::width() const -> Maybe<float>
{
  return mWidth;
}

auto TilesetView::height() const -> Maybe<float>
{
  return mHeight;
}

}  // namespace tactile
