/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include <imgui.h>
#include <imgui_internal.h>

#include "common/color.hpp"
#include "components/document.hpp"
#include "components/tile.hpp"
#include "components/tileset.hpp"
#include "components/viewport.hpp"
#include "model/event/tileset_events.hpp"
#include "model/settings.hpp"
#include "model/systems/language_system.hpp"
#include "model/systems/render_system.hpp"
#include "ui/conversions.hpp"
#include "ui/render/canvas.hpp"
#include "ui/render/canvas_renderer.hpp"
#include "ui/render/primitives.hpp"
#include "ui/viewport/document_viewport_offset_handler.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

inline constexpr Color kTileHoverColor {0x00, 0xFF, 0x00, 0xC8};
inline constexpr Color kSelectedTileColor {0x00, 0xEE, 0xEE, 0xFF};
inline constexpr Color kAnimationFrameSelectionColor {0xFF, 0x45, 0x00, 0xC8};

struct DockState final {
  Entity tileset_entity {};
  bool animation_frame_selection_mode : 1 {false};
};

inline constinit DockState gDockState;

void _show_cursor_gizmos(const ViewportMouseInfo& cursor, const CanvasInfo& canvas_info)
{
  const auto color = gDockState.animation_frame_selection_mode
                         ? kAnimationFrameSelectionColor
                         : kTileHoverColor;
  draw_shadowed_rect(as_imvec2(cursor.clamped_pos),
                     canvas_info.graphical_tile_size,
                     color,
                     2.0f);
}

void _highlight_selected_tile(const CanvasInfo& canvas, const Tileset& tileset)
{
  if (tileset.selected_tile_index.has_value()) {
    const auto tile_pos =
        TilePos::from_index(*tileset.selected_tile_index, tileset.column_count);
    const auto translated_tile_pos =
        canvas.origin_pos + as_imvec2(tile_pos.as_vec2f()) * canvas.graphical_tile_size;

    draw_shadowed_rect(translated_tile_pos,
                       canvas.graphical_tile_size,
                       kSelectedTileColor,
                       2.0f);
  }
}

void _highlight_animation_frame_selection_mode(const Strings& strings,
                                               const CanvasInfo& canvas)
{
  if (gDockState.animation_frame_selection_mode) {
    draw_rect(canvas.top_left, canvas.size, kAnimationFrameSelectionColor, 6.0f);

    const char* label = strings.animation_dock.new_animation_frame_selection_hint.c_str();
    const auto label_size = ImGui::CalcTextSize(label);

    ImVec2 label_pos;
    label_pos.x = (canvas.size.x - label_size.x) * 0.5f;
    label_pos.y = 100;

    render_shadowed_text(label, label_pos, kWhite, 2.0f);
  }
}

void _poll_mouse(const Tileset& tileset,
                 const CanvasInfo& canvas_info,
                 Dispatcher& dispatcher)
{
  const auto mouse = get_viewport_mouse_info(canvas_info);
  if (mouse.in_viewport) {
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
      const TileIndex index =
          mouse.tile_pos.col() + (mouse.tile_pos.row() * tileset.column_count);

      if (gDockState.animation_frame_selection_mode) {
        dispatcher.enqueue<AddTileAnimationFrameEvent>(index);
        gDockState.animation_frame_selection_mode = false;
      }
      else {
        dispatcher.enqueue<SelectTilesetTileEvent>(index);
      }
    }

    _show_cursor_gizmos(mouse, canvas_info);
  }
}

}  // namespace

void show_tileset_viewport(const Model& model,
                           const Entity tileset_document_entity,
                           Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);
  const auto& settings = model.get<Settings>();

  const auto& tileset_document = model.get<TilesetDocument>(tileset_document_entity);
  const auto& viewport = model.get<Viewport>(tileset_document_entity);
  const auto& tileset = model.get<Tileset>(tileset_document.tileset);

  // Reset some previous state when changing displayed tileset
  if (gDockState.tileset_entity != tileset_document.tileset) {
    gDockState.animation_frame_selection_mode = false;
    gDockState.tileset_entity = tileset_document.tileset;
  }

  const TileExtent tileset_extent {static_cast<usize>(tileset.row_count),
                                   static_cast<usize>(tileset.column_count)};
  const auto canvas = create_canvas_info(viewport, tileset.tile_size, tileset_extent);

  update_dynamic_viewport_info(tileset_document_entity, canvas, dispatcher);
  update_document_viewport_offset(canvas.size, dispatcher);

  clear_canvas(canvas, settings.get_viewport_bg_color());
  push_scissor(canvas);

  sys::render_tileset(model, canvas, tileset);

  if (Window::contains_mouse()) {
    _poll_mouse(tileset, canvas, dispatcher);
  }

  _highlight_selected_tile(canvas, tileset);
  _highlight_animation_frame_selection_mode(strings, canvas);

  pop_scissor();
}

void enable_tile_animation_frame_selection_mode()
{
  gDockState.animation_frame_selection_mode = true;
}

}  // namespace tactile::ui