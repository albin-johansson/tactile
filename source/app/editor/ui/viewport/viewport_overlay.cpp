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

#include "viewport_overlay.hpp"

#include <entt/entity/registry.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/common/ecs.hpp"
#include "core/components/layers.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/viewport/viewport_cursor_info.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile::ui {
namespace {

constexpr auto _window_flags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
    ImGuiWindowFlags_NoMove;

constexpr float _opacity = 0.35f;

void _prepare_position_and_pivot()
{
  const auto pos = ImGui::GetWindowPos();
  const auto size = ImGui::GetWindowSize();

  const auto corner = io::get_preferences().viewport_overlay_pos();
  const bool isRight =
      corner == io::OverlayPos::TopRight || corner == io::OverlayPos::BottomRight;
  const bool isBottom =
      corner == io::OverlayPos::BottomLeft || corner == io::OverlayPos::BottomRight;

  const float padding = 10.0f;

  ImVec2 next_pos{};
  next_pos.x = isRight ? (pos.x + size.x - padding) : (pos.x + padding);
  next_pos.y = isBottom ? (pos.y + size.y - padding)
                        : (pos.y + padding + ImGui::GetFrameHeightWithSpacing());

  ImVec2 next_pivot{};
  next_pivot.x = isRight ? 1.0f : 0.0f;
  next_pivot.y = isBottom ? 1.0f : 0.0f;

  ImGui::SetNextWindowPos(next_pos, ImGuiCond_Always, next_pivot);
  ImGui::SetNextWindowViewport(ImGui::GetWindowViewport()->ID);
}

void _show_mouse_tile_labels(const entt::registry& registry,
                             const ViewportCursorInfo& cursor)
{
  const auto& activeLayer = ctx_get<comp::ActiveLayer>(registry);

  if (activeLayer.entity != entt::null) {
    if (const auto* layer = registry.try_get<comp::TileLayer>(activeLayer.entity)) {
      const auto global = sys::get_tile(*layer, cursor.map_position);

      if (cursor.is_within_map && global != empty_tile) {
        ImGui::Text("Global ID: %i", global);
      }
      else {
        ImGui::TextUnformatted("Global ID: [empty]");
      }

      if (global != empty_tile) {
        const auto local = sys::convert_to_local(registry, global);
        if (cursor.is_within_map && local) {
          ImGui::Text("Local ID: %i", *local);
        }
        else {
          ImGui::TextUnformatted("Local ID: N/A");
        }
      }
    }
  }
}

void _update_overlay_context_menu()
{
  if (auto popup = scoped::Popup::for_window("##ViewportOverlayPopup"); popup.is_open()) {
    auto& prefs = io::get_preferences();
    const auto corner = prefs.viewport_overlay_pos();

    if (ImGui::MenuItem("Top-left", nullptr, corner == io::OverlayPos::TopLeft)) {
      prefs.set_viewport_overlay_pos(io::OverlayPos::TopLeft);
    }

    if (ImGui::MenuItem("Top-right", nullptr, corner == io::OverlayPos::TopRight)) {
      prefs.set_viewport_overlay_pos(io::OverlayPos::TopRight);
    }

    if (ImGui::MenuItem("Bottom-left", nullptr, corner == io::OverlayPos::BottomLeft)) {
      prefs.set_viewport_overlay_pos(io::OverlayPos::BottomLeft);
    }

    if (ImGui::MenuItem("Bottom-right", nullptr, corner == io::OverlayPos::BottomRight)) {
      prefs.set_viewport_overlay_pos(io::OverlayPos::BottomRight);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_METRICS " Show Framerate",
                        nullptr,
                        prefs.viewport_overlay_show_fps())) {
      prefs.set_viewport_overlay_show_fps(!prefs.viewport_overlay_show_fps());
    }
  }
}

}  // namespace

void update_viewport_overlay(const entt::registry& registry,
                             const ViewportCursorInfo& cursor)
{
  _prepare_position_and_pivot();

  ImGui::SetNextWindowBgAlpha(_opacity);
  scoped::Window window{"##ViewportOverlay", _window_flags};

  if (window.is_open()) {
    if (io::get_preferences().viewport_overlay_show_fps()) {
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
      ImGui::TextUnformatted("X/Y: N/A");
    }

    if (cursor.is_within_map) {
      ImGui::Text("Row/Column: (%i, %i)",
                  cursor.map_position.row(),
                  cursor.map_position.col());
    }
    else {
      ImGui::TextUnformatted("Row/Column: N/A");
    }

    ImGui::Separator();
    _show_mouse_tile_labels(registry, cursor);

    _update_overlay_context_menu();
  }
}

}  // namespace tactile::ui
