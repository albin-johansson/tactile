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
#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/documents/map_document.hpp"
#include "core/documents/tileset_document.hpp"
#include "core/events/tileset_events.hpp"
#include "core/events/viewport_events.hpp"
#include "core/model.hpp"
#include "editor/ui/common/rubber_band.hpp"
#include "editor/ui/conversions.hpp"
#include "editor/ui/rendering/graphics.hpp"
#include "editor/ui/rendering/render_info.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile::ui {
namespace {

constexpr cen::color _rubber_band_color{0, 0x44, 0xCC, 100};
constexpr cen::color _grid_color{200, 200, 200, 40};

void _update_viewport_offset(const TilesetRef& tilesetRef,
                             const ImVec2&     viewportSize,
                             entt::dispatcher& dispatcher)
{
  const auto&    tileset = tilesetRef.tileset;
  const Vector2f textureSize = tileset->texture_size();

  const Vector2f minOffset{viewportSize.x - textureSize.x,
                           viewportSize.y - textureSize.y};
  const Vector2f maxOffset{};

  const auto& limits = tilesetRef.viewport.get_limits();
  if (!limits.has_value() || minOffset != limits->min_offset) {
    dispatcher.enqueue<UpdateTilesetViewportLimitsEvent>(tileset->get_uuid(),
                                                         minOffset,
                                                         maxOffset);
  }

  ImGui::InvisibleButton("##TilesetViewInvisibleButton",
                         viewportSize,
                         ImGuiButtonFlags_MouseButtonLeft |
                             ImGuiButtonFlags_MouseButtonMiddle |
                             ImGuiButtonFlags_MouseButtonRight);

  /* This has no effect when users use touchpads, but that is handled separately */
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
    const auto&    io = ImGui::GetIO();
    const Vector2f delta{io.MouseDelta.x, io.MouseDelta.y};
    dispatcher.enqueue<OffsetTilesetViewportEvent>(tileset->get_uuid(), delta);
  }
}

void _render_selection(GraphicsCtx&  graphics,
                       const Region& selection,
                       const ImVec2& min,
                       const ImVec2& tileSize)
{
  const auto diff = selection.end - selection.begin;

  const auto origin = from_pos(selection.begin) * tileSize;
  const auto size = from_pos(diff) * tileSize;

  graphics.set_draw_color(_rubber_band_color);
  graphics.fill_rect(min + origin, size);
}

}  // namespace

void update_tileset_view(const DocumentModel& model,
                         const UUID&          tilesetId,
                         entt::dispatcher&    dispatcher)
{
  const auto& document = model.require_active_map();
  const auto& map = document.get_map();

  const auto& tilesetRef = map.get_tilesets().get_ref(tilesetId);
  const auto& tileset = tilesetRef.tileset;
  const auto& viewport = tilesetRef.viewport;

  const auto info = get_render_info(viewport, *tileset);
  _update_viewport_offset(tilesetRef, info.canvas_br - info.canvas_tl, dispatcher);

  GraphicsCtx graphics{info};
  graphics.set_draw_color(io::get_preferences().viewport_bg());
  graphics.clear();

  const auto offset = from_vec(viewport.get_offset());
  const auto tileSize = from_vec(tileset->tile_size());

  if (const auto selection = rubber_band(offset, tileSize)) {
    dispatcher.enqueue<SetTilesetSelectionEvent>(*selection);
  }

  graphics.push_clip();

  const auto position = ImGui::GetWindowDrawList()->GetClipRectMin() + offset;
  graphics.render_image(tileset->texture_id(),
                        position,
                        from_vec(tileset->texture_size()));

  const auto& selection = tilesetRef.selection;
  if (selection.has_value()) {
    _render_selection(graphics, *selection, position, tileSize);
  }

  graphics.set_line_thickness(1);
  graphics.set_draw_color(_grid_color);
  graphics.render_translated_grid();

  graphics.pop_clip();
}

}  // namespace tactile::ui
