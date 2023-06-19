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

#include "model/components/layer.hpp"
#include "model/components/map.hpp"
#include "model/event/setting_events.hpp"
#include "model/systems/language_system.hpp"
#include "model/systems/tileset_system.hpp"
#include "ui/render/canvas.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

constexpr auto kOverlayWindowFlags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
    ImGuiWindowFlags_NoMove;

constexpr float kOverlayOpacity = 0.35f;

void _prepare_position_and_pivot(const Settings& settings)
{
  const auto pos = ImGui::GetWindowPos();
  const auto size = ImGui::GetWindowSize();

  const auto corner = settings.get_viewport_overlay_pos();
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

void _push_mouse_tile_labels(const Model& model,
                             const Strings& strings,
                             const Map& map,
                             const ViewportMouseInfo& mouse)
{
  if (map.active_layer == kNullEntity) {
    return;
  }

  if (const auto* tile_layer = model.try_get<TileLayer>(map.active_layer)) {
    const auto tile_id = tile_layer->tile_at(mouse.tile_pos);

    if (mouse.in_viewport && tile_id.has_value() && tile_id != kEmptyTile) {
      ImGui::Text("%s: %i", strings.misc.global_id.c_str(), *tile_id);

      if (sys::is_valid_tile_identifier(model, map, *tile_id)) {
        const auto tile_index = sys::convert_tile_id_to_index(model, map, *tile_id);
        ImGui::Text("%s: %i", strings.misc.local_id.c_str(), tile_index.value());
      }
      else {
        ImGui::Text("%s: -", strings.misc.local_id.c_str());
      }
    }
    else {
      ImGui::Text("%s: %s", strings.misc.global_id.c_str(), strings.misc.empty.c_str());
    }
  }
}

void _push_overlay_context_menu(const Strings& strings,
                                const Settings& settings,
                                Dispatcher& dispatcher)
{
  if (auto popup = Popup::for_window("##ViewportOverlayPopup"); popup.is_open()) {
    const auto corner = settings.get_viewport_overlay_pos();

    if (ImGui::MenuItem(strings.action.top_left.c_str(),
                        nullptr,
                        corner == OverlayPos::TopLeft)) {
      dispatcher.enqueue<SetViewportOverlayPosEvent>(OverlayPos::TopLeft);
    }

    if (ImGui::MenuItem(strings.action.top_right.c_str(),
                        nullptr,
                        corner == OverlayPos::TopRight)) {
      dispatcher.enqueue<SetViewportOverlayPosEvent>(OverlayPos::TopRight);
    }

    if (ImGui::MenuItem(strings.action.bottom_left.c_str(),
                        nullptr,
                        corner == OverlayPos::BottomLeft)) {
      dispatcher.enqueue<SetViewportOverlayPosEvent>(OverlayPos::BottomLeft);
    }

    if (ImGui::MenuItem(strings.action.bottom_right.c_str(),
                        nullptr,
                        corner == OverlayPos::BottomRight)) {
      dispatcher.enqueue<SetViewportOverlayPosEvent>(OverlayPos::BottomRight);
    }

    ImGui::Separator();

    bool show_overlay_fps = settings.test_flag(SETTINGS_SHOW_VIEWPORT_OVERLAY_FPS_BIT);
    if (ImGui::MenuItem(strings.action.show_frame_rate.c_str(),
                        nullptr,
                        &show_overlay_fps)) {
      dispatcher.enqueue<SetFlagSettingEvent>(SETTINGS_SHOW_VIEWPORT_OVERLAY_FPS_BIT,
                                              show_overlay_fps);
    }
  }
}

}  // namespace

void show_map_viewport_overlay(const Model& model,
                               const Map& map,
                               const ViewportMouseInfo& mouse,
                               Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);
  const auto& settings = model.get<Settings>();

  _prepare_position_and_pivot(settings);

  ImGui::SetNextWindowBgAlpha(kOverlayOpacity);
  const Window window {"##ViewportOverlay", kOverlayWindowFlags};

  if (window.is_open()) {
    if (settings.test_flag(SETTINGS_SHOW_VIEWPORT_OVERLAY_FPS_BIT)) {
      const auto& io = ImGui::GetIO();
      ImGui::Text("%.2f ms (%.1f FPS)", 1'000.0f * io.DeltaTime, io.Framerate);
      ImGui::Separator();
    }

    if (ImGui::IsMousePosValid()) {
      ImGui::Text("X/Y: (%.0f, %.0f)", mouse.scaled_pos.x, mouse.scaled_pos.y);
    }
    else {
      ImGui::TextUnformatted("X/Y: --");
    }

    if (mouse.in_viewport) {
      ImGui::Text("%s/%s: (%i, %i)",
                  strings.misc.row.c_str(),
                  strings.misc.column.c_str(),
                  mouse.tile_pos.row(),
                  mouse.tile_pos.col());
    }
    else {
      ImGui::Text("%s/%s: --", strings.misc.row.c_str(), strings.misc.column.c_str());
    }

    ImGui::Separator();
    _push_mouse_tile_labels(model, strings, map, mouse);
    _push_overlay_context_menu(strings, settings, dispatcher);
  }
}

}  // namespace tactile::ui
