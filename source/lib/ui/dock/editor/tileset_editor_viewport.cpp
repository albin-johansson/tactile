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

#include "tileset_editor_viewport.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "common/color.hpp"
#include "common/util/lookup.hpp"
#include "model/documents/document_components.hpp"
#include "model/events/tileset_events.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"
#include "model/services/settings/settings.hpp"
#include "model/tiles/tile_components.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "model/tilesets/tileset_ops.hpp"
#include "model/view/render_system.hpp"
#include "model/viewports/viewport_components.hpp"
#include "ui/conversions.hpp"
#include "ui/dock/editor/document_viewport_offset_handler.hpp"
#include "ui/render/canvas.hpp"
#include "ui/render/canvas_renderer.hpp"
#include "ui/render/primitives.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile {
namespace {

inline constexpr Color kTileHoverColor {0x00, 0xFF, 0x00, 0xC8};
inline constexpr Color kSelectedTileColor {0x00, 0xEE, 0xEE, 0xFF};
inline constexpr Color kAnimationFrameSelectionColor {0xFF, 0x45, 0x00, 0xC8};

void _draw_cursor_gizmos(const TilesetEditorViewportState& state,
                         const ViewportMouseInfo& cursor,
                         const ui::CanvasInfo& canvas_info)
{
  const auto color = state.animation_frame_selection_mode ? kAnimationFrameSelectionColor
                                                          : kTileHoverColor;
  ui::draw_shadowed_rect(as_imvec2(cursor.clamped_pos),
                         canvas_info.graphical_tile_size,
                         color,
                         2.0f);
}

void _highlight_selected_tile(const ui::CanvasInfo& canvas, const Tileset& tileset)
{
  if (tileset.selected_tile_index.has_value()) {
    const auto tile_pos =
        TilePos::from_index(*tileset.selected_tile_index, tileset.column_count);
    const auto translated_tile_pos =
        canvas.origin_pos + as_imvec2(tile_pos.as_vec2f()) * canvas.graphical_tile_size;

    ui::draw_shadowed_rect(translated_tile_pos,
                           canvas.graphical_tile_size,
                           kSelectedTileColor,
                           2.0f);
  }
}

void _highlight_animation_frame_selection_mode(const TilesetEditorViewportState& state,
                                               const Strings& strings,
                                               const ui::CanvasInfo& canvas)
{
  if (state.animation_frame_selection_mode) {
    ui::draw_rect(canvas.top_left, canvas.size, kAnimationFrameSelectionColor, 6.0f);

    const char* label = strings.animation_dock.new_animation_frame_selection_hint.c_str();
    const auto label_size = ImGui::CalcTextSize(label);

    ImVec2 label_pos;
    label_pos.x = (canvas.size.x - label_size.x) * 0.5f;
    label_pos.y = 100;

    ui::render_shadowed_text(label, label_pos, kWhite, 2.0f);
  }
}

void _poll_mouse(TilesetEditorViewportState& state,
                 const Entity tileset_document_entity,
                 const Tileset& tileset,
                 const ui::CanvasInfo& canvas_info,
                 Dispatcher& dispatcher)
{
  using namespace std::chrono_literals;
  const auto mouse = get_viewport_mouse_info(canvas_info);

  if (mouse.over_content) {
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
      const auto mouse_tile_index = sys::tile_index_at(tileset, mouse.tile_pos);

      if (state.animation_frame_selection_mode) {
        dispatcher.enqueue<AddAnimationFrameEvent>(sys::get_active_tile(tileset),
                                                   mouse_tile_index,
                                                   100ms);
        state.animation_frame_selection_mode = false;
      }
      else {
        dispatcher.enqueue<SelectTilesetTileEvent>(tileset_document_entity,
                                                   mouse_tile_index);
      }
    }

    _draw_cursor_gizmos(state, mouse, canvas_info);
  }
}

}  // namespace

void push_tileset_editor_viewport(ModelView& model,
                                  TilesetEditorViewportState& state,
                                  const Entity tileset_document_entity)
{
  auto& dispatcher = model.get_dispatcher();
  const auto& registry = model.get_registry();
  const auto& strings = model.get_language_strings();
  const auto& settings = model.get_settings();

  const auto& tileset_document = registry.get<TilesetDocument>(tileset_document_entity);
  const auto& viewport = registry.get<Viewport>(tileset_document_entity);
  const auto& tileset = registry.get<Tileset>(tileset_document.tileset);

  // Reset some previous state when changing displayed tileset
  if (state.tileset_entity != tileset_document.tileset) {
    state.animation_frame_selection_mode = false;
    state.tileset_entity = tileset_document.tileset;
  }

  // TODO store TileExtent in Tileset component
  const TileExtent tileset_extent {static_cast<usize>(tileset.row_count),
                                   static_cast<usize>(tileset.column_count)};
  const auto canvas = ui::create_canvas_info(viewport, tileset.tile_size, tileset_extent);

  ui::update_dynamic_viewport_info(tileset_document_entity, canvas, dispatcher);
  ui::update_document_viewport_offset(tileset_document_entity, canvas.size, dispatcher);

  ui::clear_canvas(canvas, settings.get_viewport_bg_color());
  ui::push_scissor(canvas);

  sys::render_tileset(registry, canvas, tileset);

  if (ui::Window::contains_mouse()) {
    _poll_mouse(state, tileset_document_entity, tileset, canvas, dispatcher);
  }

  _highlight_selected_tile(canvas, tileset);
  _highlight_animation_frame_selection_mode(state, strings, canvas);

  ui::pop_scissor();
}

}  // namespace tactile