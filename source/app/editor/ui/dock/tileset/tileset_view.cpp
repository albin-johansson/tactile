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

#include "editor/ui/conversions.hpp"
#include "editor/ui/render/graphics.hpp"
#include "editor/ui/render/render_info.hpp"
#include "editor/ui/style/colors.hpp"
#include "editor/ui/widget/rubber_band.hpp"
#include "io/proto/preferences.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/tileset_events.hpp"
#include "model/event/viewport_events.hpp"
#include "model/model.hpp"

namespace tactile::ui {
namespace {

constexpr uint32 rubber_band_color = IM_COL32(0, 0x44, 0xCC, 100);
constexpr uint32 grid_color = IM_COL32(200, 200, 200, 40);

void update_viewport_offset(const TilesetRef& tileset_ref,
                            const ImVec2& viewport_size,
                            entt::dispatcher& dispatcher)
{
  const auto& tileset = tileset_ref.view_tileset();
  const Float2 texture_size = tileset.texture_size();

  const Float2 min_offset {viewport_size.x - texture_size.x,
                           viewport_size.y - texture_size.y};
  const Float2 max_offset {};

  const auto& limits = tileset_ref.get_viewport().limits();
  if (!limits.has_value() || min_offset != limits->min_offset) {
    dispatcher.enqueue<UpdateTilesetViewportLimitsEvent>(tileset.uuid(),
                                                         min_offset,
                                                         max_offset);
  }

  ImGui::InvisibleButton("##TilesetViewInvisibleButton",
                         viewport_size,
                         ImGuiButtonFlags_MouseButtonLeft |
                             ImGuiButtonFlags_MouseButtonMiddle |
                             ImGuiButtonFlags_MouseButtonRight);

  /* This has no effect when users use touchpads, but that is handled separately */
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
    const auto& io = ImGui::GetIO();
    const Float2 delta {io.MouseDelta.x, io.MouseDelta.y};
    dispatcher.enqueue<OffsetTilesetViewportEvent>(tileset.uuid(), delta);
  }
}

void render_selection(Graphics& graphics,
                      const Region& selection,
                      const ImVec2& min,
                      const ImVec2& tile_size)
{
  const auto diff = selection.end - selection.begin;

  const auto origin = from_pos(selection.begin) * tile_size;
  const auto size = from_pos(diff) * tile_size;

  graphics.fill_rect(min + origin, size, rubber_band_color);
}

}  // namespace

void update_tileset_view(const DocumentModel& model,
                         const UUID& tileset_id,
                         entt::dispatcher& dispatcher)
{
  const auto& document = model.require_active_map();
  const auto& map = document.get_map();

  const auto& tileset_ref = map.tileset_bundle().get_ref(tileset_id);
  const auto& tileset = tileset_ref.view_tileset();
  const auto& viewport = tileset_ref.get_viewport();

  const auto info = get_render_info(viewport, tileset);
  update_viewport_offset(tileset_ref, info.canvas_br - info.canvas_tl, dispatcher);

  Graphics graphics {info};
  graphics.clear(color_to_u32(io::get_preferences().viewport_background));

  const auto offset = from_vec(viewport.get_offset());
  const auto tile_size = from_vec(tileset.tile_size());

  if (const auto selection = rubber_band(offset, tile_size)) {
    dispatcher.enqueue<SetTilesetSelectionEvent>(*selection);
  }

  graphics.push_clip();

  const auto position = ImGui::GetWindowDrawList()->GetClipRectMin() + offset;
  graphics.render_image(tileset.texture_id(), position, from_vec(tileset.texture_size()));

  const auto& selection = tileset_ref.get_selection();
  if (selection.has_value()) {
    render_selection(graphics, *selection, position, tile_size);
  }

  graphics.render_translated_grid(grid_color);
  graphics.pop_clip();
}

}  // namespace tactile::ui
