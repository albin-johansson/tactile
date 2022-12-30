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

#include "tileset_viewport.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/tile/tile.hpp"
#include "graphics/graphics.hpp"
#include "graphics/render_info.hpp"
#include "graphics/render_tileset.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/tileset_events.hpp"
#include "ui/conversions.hpp"
#include "ui/viewport/document_viewport_offset_handler.hpp"
#include "ui/viewport/viewport_cursor_info.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

inline constexpr uint32 tile_hover_color = IM_COL32(0, 0xFF, 0, 200);
inline constexpr uint32 selected_tile_color = IM_COL32(0, 0xEE, 0xEE, 0xFF);
inline constexpr uint32 animation_frame_selection_color = IM_COL32(0xFF, 0x45, 0x00, 200);

struct DockState final {
  UUID tileset_id {};
  bool animation_frame_selection_mode : 1 {false};
};

inline constinit DockState dock_state;

void ui_cursor_gizmos(const ViewportCursorInfo& cursor, const RenderInfo& render_info)
{
  const auto color = dock_state.animation_frame_selection_mode
                         ? animation_frame_selection_color
                         : tile_hover_color;
  draw_shadowed_rect(cursor.clamped_position, render_info.grid_size, color, 2.0f);
}

void ui_highlight_selected_tile(const RenderInfo& info, const Tileset& tileset)
{
  if (const auto tile_index = tileset.get_selected_tile()) {
    const auto tile_pos = TilePos::from_index(*tile_index, tileset.column_count());
    const auto translated_tile_pos = info.origin + from_pos(tile_pos) * info.grid_size;

    draw_shadowed_rect(translated_tile_pos, info.grid_size, selected_tile_color, 2.0f);
  }
}

void ui_highlight_animation_frame_selection_mode(const Strings& lang,
                                                 const RenderInfo& info)
{
  if (dock_state.animation_frame_selection_mode) {
    draw_rect(info.canvas_tl, info.canvas_size, animation_frame_selection_color, 6.0f);

    const char* label = lang.animation_dock.new_animation_frame_selection_hint.c_str();
    const auto label_size = ImGui::CalcTextSize(label);

    const float label_x = (info.canvas_size.x - label_size.x) * 0.5f;
    const float label_y = 100;

    render_shadowed_text(label, {label_x, label_y}, IM_COL32_WHITE, 2.0f);
  }
}

void ui_poll_mouse(const Tileset& tileset,
                   const RenderInfo& render_info,
                   entt::dispatcher& dispatcher)
{
  const auto cursor = get_viewport_cursor_info(render_info);
  if (cursor.is_within_map) {
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
      const TileIndex index = cursor.map_position.col() +
                              (cursor.map_position.row() * tileset.column_count());

      if (dock_state.animation_frame_selection_mode) {
        dispatcher.enqueue<AddTileAnimationFrameEvent>(index);
        dock_state.animation_frame_selection_mode = false;
      }
      else {
        dispatcher.enqueue<SelectTilesetTileEvent>(index);
      }
    }

    ui_cursor_gizmos(cursor, render_info);
  }
}

}  // namespace

void show_tileset_viewport(const TilesetDocument& document, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();
  const auto& tileset = document.get_tileset();
  const auto& viewport = document.get_viewport();

  // Reset some previous state when changing displayed tileset
  if (dock_state.tileset_id != tileset.get_uuid()) {
    dock_state.animation_frame_selection_mode = false;

    dock_state.tileset_id = tileset.get_uuid();
  }

  const auto render_info = get_render_info(viewport, tileset);
  update_document_viewport_offset(render_info.canvas_size, dispatcher);

  Graphics graphics {render_info};
  graphics.clear(to_u32(io::get_preferences().viewport_background));

  graphics.push_canvas_clip();
  render_tileset(graphics, document);

  if (Window::contains_mouse()) {
    ui_poll_mouse(tileset, render_info, dispatcher);
  }

  ui_highlight_selected_tile(render_info, tileset);
  ui_highlight_animation_frame_selection_mode(lang, render_info);

  graphics.pop_clip();
}

void enable_tile_animation_frame_selection_mode()
{
  dock_state.animation_frame_selection_mode = true;
}

}  // namespace tactile::ui