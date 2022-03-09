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

#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/layers.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "editor/gui/scoped.hpp"
#include "io/persistence/preferences.hpp"
#include "viewport_cursor_info.hpp"

namespace tactile {
namespace {

constexpr auto gFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                        ImGuiWindowFlags_AlwaysAutoResize |
                        ImGuiWindowFlags_NoSavedSettings |
                        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
                        ImGuiWindowFlags_NoMove;

constexpr float gOpacity = 0.35f;

void PreparePositionAndPivot()
{
  const auto pos = ImGui::GetWindowPos();
  const auto size = ImGui::GetWindowSize();

  const auto corner = get_preferences().viewport_overlay_pos();
  const bool isRight =
      corner == OverlayPos::top_right || corner == OverlayPos::bottom_right;
  const bool isBottom =
      corner == OverlayPos::bottom_left || corner == OverlayPos::bottom_right;

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

void UpdateMouseCoordinateLabels(const ViewportCursorInfo& cursor)
{
  if (ImGui::IsMousePosValid()) {
    ImGui::Text("X/Y: (%.0f, %.0f)", cursor.scaled_position.x, cursor.scaled_position.y);
  }
  else {
    ImGui::TextUnformatted("X/Y: N/A");
  }
}

void UpdateMouseRowColumnLabels(const ViewportCursorInfo& cursor)
{
  if (cursor.is_within_map) {
    ImGui::Text("Row/Column: (%i, %i)",
                cursor.map_position.row(),
                cursor.map_position.col());
  }
  else {
    ImGui::TextUnformatted("Row/Column: N/A");
  }
}

void UpdateMouseTileLabels(const entt::registry& registry,
                           const ViewportCursorInfo& cursor)
{
  const auto& activeLayer = registry.ctx<comp::ActiveLayer>();

  if (activeLayer.entity != entt::null) {
    if (registry.all_of<comp::TileLayer>(activeLayer.entity)) {
      ImGui::Separator();

      const auto& layer = registry.get<comp::TileLayer>(activeLayer.entity);
      const auto global = sys::get_tile(layer, cursor.map_position);

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

void UpdateOverlayContextMenu()
{
  if (auto popup = scoped::Popup::for_window("##ViewportOverlayPopup"); popup.is_open()) {
    auto& prefs = get_preferences();
    const auto corner = prefs.viewport_overlay_pos();

    if (ImGui::MenuItem("Top-left", nullptr, corner == OverlayPos::top_left)) {
      prefs.set_viewport_overlay_pos(OverlayPos::top_left);
    }

    if (ImGui::MenuItem("Top-right", nullptr, corner == OverlayPos::top_right)) {
      prefs.set_viewport_overlay_pos(OverlayPos::top_right);
    }

    if (ImGui::MenuItem("Bottom-left", nullptr, corner == OverlayPos::bottom_left)) {
      prefs.set_viewport_overlay_pos(OverlayPos::bottom_left);
    }

    if (ImGui::MenuItem("Bottom-right", nullptr, corner == OverlayPos::bottom_right)) {
      prefs.set_viewport_overlay_pos(OverlayPos::bottom_right);
    }
  }
}

}  // namespace

void UpdateViewportOverlay(const entt::registry& registry,
                           const ViewportCursorInfo& cursor)
{
  PreparePositionAndPivot();

  ImGui::SetNextWindowBgAlpha(gOpacity);
  scoped::Window window{"##ViewportOverlay", gFlags};

  if (window.is_open()) {
    UpdateMouseCoordinateLabels(cursor);
    UpdateMouseRowColumnLabels(cursor);
    UpdateMouseTileLabels(registry, cursor);

    UpdateOverlayContextMenu();
  }
}

}  // namespace tactile
