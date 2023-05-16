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

#include "map_viewport_overlay.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/layer.hpp"
#include "core/map.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/settings.hpp"
#include "model/systems/tileset_system.hpp"
#include "ui/viewport/viewport_cursor_info.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

constexpr auto kOverlayWindowFlags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
    ImGuiWindowFlags_NoMove;

constexpr float kOverlayOpacity = 0.35f;

void _prepare_position_and_pivot()
{
  const auto pos = ImGui::GetWindowPos();
  const auto size = ImGui::GetWindowSize();

  const auto corner = get_settings().get_viewport_overlay_pos();
  const bool is_right =
      corner == OverlayPos::TopRight || corner == OverlayPos::BottomRight;
  const bool is_bottom =
      corner == OverlayPos::BottomLeft || corner == OverlayPos::BottomRight;

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

void _show_mouse_tile_labels(const Model& model,
                             const Map& map,
                             const ViewportCursorInfo& cursor)
{
  if (map.active_layer == kNullEntity) {
    return;
  }

  const auto& lang = get_current_language();

  if (const auto* tile_layer = model.try_get<TileLayer>(map.active_layer)) {
    const auto tile_id = tile_layer->tile_at(cursor.map_position);

    if (cursor.is_within_map && tile_id.has_value() && tile_id != kEmptyTile) {
      ImGui::Text("%s: %i", lang.misc.global_id.c_str(), *tile_id);

      if (sys::is_valid_tile_identifier(model, map, *tile_id)) {
        const auto tile_index = sys::convert_tile_id_to_index(model, map, *tile_id);
        ImGui::Text("%s: %i", lang.misc.local_id.c_str(), tile_index.value());
      }
      else {
        ImGui::Text("%s: -", lang.misc.local_id.c_str());
      }
    }
    else {
      ImGui::Text("%s: %s", lang.misc.global_id.c_str(), lang.misc.empty.c_str());
    }
  }
}

void _show_overlay_context_menu()
{
  if (auto popup = Popup::for_window("##ViewportOverlayPopup"); popup.is_open()) {
    const auto& lang = get_current_language();

    auto& settings = get_global_settings();
    const auto corner = settings.get_viewport_overlay_pos();

    if (ImGui::MenuItem(lang.action.top_left.c_str(),
                        nullptr,
                        corner == OverlayPos::TopLeft)) {
      settings.set_viewport_overlay_pos(OverlayPos::TopLeft);
    }

    if (ImGui::MenuItem(lang.action.top_right.c_str(),
                        nullptr,
                        corner == OverlayPos::TopRight)) {
      settings.set_viewport_overlay_pos(OverlayPos::TopRight);
    }

    if (ImGui::MenuItem(lang.action.bottom_left.c_str(),
                        nullptr,
                        corner == OverlayPos::BottomLeft)) {
      settings.set_viewport_overlay_pos(OverlayPos::BottomLeft);
    }

    if (ImGui::MenuItem(lang.action.bottom_right.c_str(),
                        nullptr,
                        corner == OverlayPos::BottomRight)) {
      settings.set_viewport_overlay_pos(OverlayPos::BottomRight);
    }

    ImGui::Separator();

    bool show_overlay_fps = settings.test_flag(SETTINGS_SHOW_VIEWPORT_OVERLAY_FPS_BIT);
    if (ImGui::MenuItem(lang.action.show_frame_rate.c_str(),
                        nullptr,
                        &show_overlay_fps)) {
      settings.set_flag(SETTINGS_SHOW_VIEWPORT_OVERLAY_FPS_BIT, show_overlay_fps);
    }
  }
}

}  // namespace

void show_map_viewport_overlay(const Model& model,
                               const Map& map,
                               const ViewportCursorInfo& cursor)
{
  _prepare_position_and_pivot();

  ImGui::SetNextWindowBgAlpha(kOverlayOpacity);
  const Window window {"##ViewportOverlay", kOverlayWindowFlags};

  if (window.is_open()) {
    const auto& lang = get_current_language();

    if (get_settings().test_flag(SETTINGS_SHOW_VIEWPORT_OVERLAY_FPS_BIT)) {
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
    _show_mouse_tile_labels(model, map, cursor);

    _show_overlay_context_menu();
  }
}

}  // namespace tactile::ui
