// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tileset_viewport.hpp"

#include <cmath>  // round

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/tile/tile.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/tileset_events.hpp"
#include "model/event/viewport_events.hpp"
#include "model/settings.hpp"
#include "tactile/core/meta/color.hpp"
#include "ui/conversions.hpp"
#include "ui/render/primitives.hpp"
#include "ui/render/renderer.hpp"
#include "ui/viewport/document_viewport_offset_handler.hpp"
#include "ui/viewport/viewport_cursor_info.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

inline constexpr Color kTileHoverColor {0, 0xFF, 0, 200};
inline constexpr Color kSelectedTileColor {0, 0xEE, 0xEE, 0xFF};
inline constexpr Color kAnimationFrameSelectionColor {0xFF, 0x45, 0x00, 200};

struct DockState final {
  UUID tileset_id {};
  bool center_viewport : 1 {false};
  bool animation_frame_selection_mode : 1 {false};
};

inline constinit DockState gDockState;

void ui_cursor_gizmos(const ViewportCursorInfo& cursor, const CanvasInfo& canvas_info)
{
  const auto color = gDockState.animation_frame_selection_mode
                         ? kAnimationFrameSelectionColor
                         : kTileHoverColor;
  draw_shadowed_rect(as_float2(cursor.clamped_position),
                     canvas_info.grid_size,
                     color,
                     2.0f);
}

void ui_highlight_selected_tile(const CanvasInfo& canvas_info, const Tileset& tileset)
{
  if (const auto tile_index = tileset.get_selected_tile()) {
    const auto tile_pos = TilePos::from_index(*tile_index, tileset.column_count());
    const auto translated_tile_pos =
        canvas_info.origin + tile_pos.as_vec2f() * canvas_info.grid_size;

    draw_shadowed_rect(translated_tile_pos,
                       canvas_info.grid_size,
                       kSelectedTileColor,
                       2.0f);
  }
}

void ui_highlight_animation_frame_selection_mode(const Strings& lang,
                                                 const CanvasInfo& canvas_info)
{
  if (gDockState.animation_frame_selection_mode) {
    draw_rect(canvas_info.top_left,
              canvas_info.size,
              kAnimationFrameSelectionColor,
              6.0f);

    const char* label = lang.animation_dock.new_animation_frame_selection_hint.c_str();
    const auto label_size = ImGui::CalcTextSize(label);

    const float label_x = (canvas_info.size.x() - label_size.x) * 0.5f;
    const float label_y = 100;

    render_shadowed_text(label, Float2 {label_x, label_y}, kColorWhite, 2.0f);
  }
}

void center_viewport(const CanvasInfo& canvas_info,
                     const Viewport& viewport,
                     entt::dispatcher& dispatcher)
{
  const auto raw_delta =
      ((canvas_info.size - canvas_info.content_size) * 0.5f) - viewport.get_offset();
  const auto delta = round(raw_delta);
  dispatcher.enqueue<OffsetDocumentViewportEvent>(delta);
}

void ui_poll_mouse(const Tileset& tileset,
                   const CanvasInfo& canvas_info,
                   entt::dispatcher& dispatcher)
{
  const auto cursor = get_viewport_cursor_info(canvas_info);
  if (cursor.is_within_map) {
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
      const TileIndex index = cursor.map_position.col() +
                              (cursor.map_position.row() * tileset.column_count());

      if (gDockState.animation_frame_selection_mode) {
        dispatcher.enqueue<AddTileAnimationFrameEvent>(index);
        gDockState.animation_frame_selection_mode = false;
      }
      else {
        dispatcher.enqueue<SelectTilesetTileEvent>(index);
      }
    }

    ui_cursor_gizmos(cursor, canvas_info);
  }
}

}  // namespace

void show_tileset_viewport(const TilesetDocument& document, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();
  const auto& tileset = document.get_tileset();
  const auto& viewport = document.get_viewport();

  // Reset some previous state when changing displayed tileset
  if (gDockState.tileset_id != tileset.get_uuid()) {
    gDockState.animation_frame_selection_mode = false;

    gDockState.tileset_id = tileset.get_uuid();
  }

  const Renderer renderer {viewport, tileset};
  const auto& canvas_info = renderer.get_canvas_info();

  update_document_viewport_offset(as_imvec2(canvas_info.size), dispatcher);

  renderer.clear(get_settings().get_viewport_bg_color());
  renderer.push_clip();

  if (gDockState.center_viewport) {
    center_viewport(canvas_info, viewport, dispatcher);
    gDockState.center_viewport = false;
  }

  renderer.render_tileset(tileset);

  if (Window::contains_mouse()) {
    ui_poll_mouse(tileset, canvas_info, dispatcher);
  }

  ui_highlight_selected_tile(canvas_info, tileset);
  ui_highlight_animation_frame_selection_mode(lang, canvas_info);

  renderer.pop_clip();
}

void center_tileset_viewport()
{
  gDockState.center_viewport = true;
}

void enable_tile_animation_frame_selection_mode()
{
  gDockState.animation_frame_selection_mode = true;
}

}  // namespace tactile::ui
