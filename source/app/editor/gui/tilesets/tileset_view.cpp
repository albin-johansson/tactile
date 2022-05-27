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

#include <centurion/color.hpp>
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/common/ecs.hpp"
#include "core/components/texture.hpp"
#include "core/components/tiles.hpp"
#include "core/components/viewport.hpp"
#include "core/systems/tileset_system.hpp"
#include "editor/documents/map_document.hpp"
#include "editor/documents/tileset_document.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/common/rubber_band.hpp"
#include "editor/gui/rendering/graphics.hpp"
#include "editor/gui/rendering/render_info.hpp"
#include "editor/model.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/assert.hpp"

namespace tactile {
namespace {

constexpr cen::color _rubber_band_color{0, 0x44, 0xCC, 100};
constexpr cen::color _grid_color{200, 200, 200, 40};

constinit Maybe<float> _view_width;
constinit Maybe<float> _view_height;

void _update_viewport_offset(const entt::entity tilesetEntity,
                             entt::dispatcher& dispatcher,
                             const ImVec2& viewportSize)
{
  ImGui::InvisibleButton("##TilesetViewInvisibleButton",
                         viewportSize,
                         ImGuiButtonFlags_MouseButtonLeft |
                             ImGuiButtonFlags_MouseButtonMiddle |
                             ImGuiButtonFlags_MouseButtonRight);

  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
    const auto& io = ImGui::GetIO();
    dispatcher.enqueue<OffsetBoundViewportEvent>(tilesetEntity,
                                                 io.MouseDelta.x,
                                                 io.MouseDelta.y,
                                                 viewportSize.x,
                                                 viewportSize.y);
  }
}

void _render_selection(GraphicsCtx& graphics,
                       const Region& selection,
                       const ImVec2& min,
                       const ImVec2& tileSize)
{
  const auto diff = selection.end - selection.begin;

  const ImVec2 origin{static_cast<float>(selection.begin.col()) * tileSize.x,
                      static_cast<float>(selection.begin.row()) * tileSize.y};

  const ImVec2 size{static_cast<float>(diff.col()) * tileSize.x,
                    static_cast<float>(diff.row()) * tileSize.y};

  graphics.set_draw_color(_rubber_band_color);
  graphics.fill_rect(min + origin, size);
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

void update_tileset_view(const DocumentModel& model,
                         const UUID& tilesetId,
                         entt::dispatcher& dispatcher)
{
  const auto activeDocumentId = model.active_document_id().value();

  const auto mapDoc = model.get_map(activeDocumentId);
  const auto tilesetDoc = model.get_tileset(tilesetId);

  const auto& mapRegistry = mapDoc->get_registry();
  const auto& tilesetInfo = tilesetDoc->info();
  const auto& texture = tilesetDoc->texture();

  const auto tilesetEntity = sys::find_tileset(mapRegistry, tilesetId);
  const auto& viewport = checked_get<comp::Viewport>(mapRegistry, tilesetEntity);

  const auto region = ImGui::GetContentRegionAvail();
  _view_width = region.x;
  _view_height = region.y;

  const auto info = get_render_info(viewport, tilesetInfo);
  _update_viewport_offset(tilesetEntity, dispatcher, info.canvas_br - info.canvas_tl);

  GraphicsCtx graphics{info};
  graphics.set_draw_color(get_preferences().viewport_bg());
  graphics.clear();

  const ImVec2 offset{viewport.x_offset, viewport.y_offset};
  const ImVec2 tileSize = {static_cast<float>(tilesetInfo.tile_width),
                           static_cast<float>(tilesetInfo.tile_height)};

  if (const auto selection = rubber_band(offset, tileSize)) {
    dispatcher.enqueue<SetTilesetSelectionEvent>(*selection);
  }

  graphics.push_clip();

  const auto position = ImGui::GetWindowDrawList()->GetClipRectMin() + offset;
  _render_tileset_image(graphics, texture, position);

  if (const auto& selection =
          checked_get<comp::TilesetSelection>(mapRegistry, tilesetEntity);
      selection.region) {
    _render_selection(graphics, *selection.region, position, tileSize);
  }

  graphics.set_line_thickness(1);
  graphics.set_draw_color(_grid_color);
  graphics.render_translated_grid();

  graphics.pop_clip();
}

auto get_tileset_view_width() -> Maybe<float>
{
  return _view_width;
}

auto get_tileset_view_height() -> Maybe<float>
{
  return _view_height;
}

}  // namespace tactile
