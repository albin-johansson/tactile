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

#include "map_viewport_overlay.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/layer/tile_layer.hpp"
#include "core/map.hpp"
#include "editor/ui/viewport/viewport_cursor_info.hpp"
#include "editor/ui/widget/scoped.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::ui {
namespace {

constexpr auto overlay_window_flags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
    ImGuiWindowFlags_NoMove;

constexpr float overlay_opacity = 0.35f;

void prepare_position_and_pivot()
{
  const auto pos = ImGui::GetWindowPos();
  const auto size = ImGui::GetWindowSize();

  const auto corner = io::get_preferences().viewport_overlay_pos;
  const bool is_right =
      corner == io::OverlayPos::TopRight || corner == io::OverlayPos::BottomRight;
  const bool is_bottom =
      corner == io::OverlayPos::BottomLeft || corner == io::OverlayPos::BottomRight;

  const float padding = 10.0f;

  ImVec2 next_pos {};
  next_pos.x = is_right ? (pos.x + size.x - padding) : (pos.x + padding);
  next_pos.y = is_bottom ? (pos.y + size.y - padding)
                         : (pos.y + padding + ImGui::GetFrameHeightWithSpacing());

  ImVec2 next_pivot {};
  next_pivot.x = is_right ? 1.0f : 0.0f;
  next_pivot.y = is_bottom ? 1.0f : 0.0f;

  ImGui::SetNextWindowPos(next_pos, ImGuiCond_Always, next_pivot);
  ImGui::SetNextWindowViewport(ImGui::GetWindowViewport()->ID);
}

void show_mouse_tile_labels(const Map& map, const ViewportCursorInfo& cursor)
{
  const auto layer_id = map.active_layer_id();
  if (!layer_id) {
    return;
  }

  const auto& lang = get_current_language();

  if (const auto* layer = map.invisible_root().as_tile_layer(*layer_id)) {
    const auto tile_id = layer->tile_at(cursor.map_position);

    if (cursor.is_within_map && tile_id != empty_tile) {
      ImGui::Text("%s: %i", lang.misc.global_id.c_str(), tile_id);

      const auto& tilesets = map.tileset_bundle();
      const auto tile_index = tilesets.to_tile_index(tile_id);
      ImGui::Text("%s: %i", lang.misc.local_id.c_str(), tile_index);
    }
    else {
      ImGui::Text("%s: %s", lang.misc.global_id.c_str(), lang.misc.empty.c_str());
    }
  }
}

void update_overlay_context_menu()
{
  if (auto popup = Popup::for_window("##ViewportOverlayPopup"); popup.is_open()) {
    const auto& lang = get_current_language();

    auto& prefs = io::get_preferences();
    const auto corner = prefs.viewport_overlay_pos;

    if (ImGui::MenuItem(lang.action.top_left.c_str(),
                        nullptr,
                        corner == io::OverlayPos::TopLeft)) {
      prefs.viewport_overlay_pos = io::OverlayPos::TopLeft;
    }

    if (ImGui::MenuItem(lang.action.top_right.c_str(),
                        nullptr,
                        corner == io::OverlayPos::TopRight)) {
      prefs.viewport_overlay_pos = io::OverlayPos::TopRight;
    }

    if (ImGui::MenuItem(lang.action.bottom_left.c_str(),
                        nullptr,
                        corner == io::OverlayPos::BottomLeft)) {
      prefs.viewport_overlay_pos = io::OverlayPos::BottomLeft;
    }

    if (ImGui::MenuItem(lang.action.bottom_right.c_str(),
                        nullptr,
                        corner == io::OverlayPos::BottomRight)) {
      prefs.viewport_overlay_pos = io::OverlayPos::BottomRight;
    }

    ImGui::Separator();

    ImGui::MenuItem(lang.action.show_frame_rate.c_str(),
                    nullptr,
                    &prefs.show_viewport_overlay_fps);
  }
}

}  // namespace

void update_map_viewport_overlay(const Map& map, const ViewportCursorInfo& cursor)
{
  prepare_position_and_pivot();

  ImGui::SetNextWindowBgAlpha(overlay_opacity);
  Window window {"##ViewportOverlay", overlay_window_flags};

  if (window.is_open()) {
    const auto& lang = get_current_language();

    if (io::get_preferences().show_viewport_overlay_fps) {
      const auto& io = ImGui::GetIO();
      ImGui::Text("%.2f ms (%.1f FPS)", 1'000.0f * io.DeltaTime, io.Framerate);
      ImGui::Separator();
    }

    if (ImGui::IsMousePosValid()) {
      ImGui::Text("X/Y: (%.0f, %.0f)",
                  cursor.scaled_position.x,
                  cursor.scaled_position.y);
    }
    else {
      ImGui::TextUnformatted("X/Y: --");
    }

    if (cursor.is_within_map) {
      ImGui::Text("%s/%s: (%i, %i)",
                  lang.misc.row.c_str(),
                  lang.misc.column.c_str(),
                  cursor.map_position.row(),
                  cursor.map_position.col());
    }
    else {
      ImGui::Text("%s/%s: --", lang.misc.row.c_str(), lang.misc.column.c_str());
    }

    ImGui::Separator();
    show_mouse_tile_labels(map, cursor);

    update_overlay_context_menu();
  }
}

}  // namespace tactile::ui
