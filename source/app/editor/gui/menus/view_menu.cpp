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

#include "view_menu.hpp"

#include <entt/entt.hpp>
#include <imgui.h>

#include "cfg/fonts.hpp"
#include "editor/events/misc_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/dock_space.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile {
namespace {

void _update_widgets_menu(const bool hasActiveMap)
{
  if (scoped::Menu menu{"Widgets", hasActiveMap}; menu.is_open()) {
    if (ImGui::MenuItem("Reset Layout")) {
      reset_layout();
    }

    ImGui::Separator();

    auto& prefs = get_preferences();

    if (ImGui::MenuItem("Properties", nullptr, prefs.is_property_dock_visible())) {
      prefs.set_property_dock_visible(!prefs.is_property_dock_visible());
    }

    if (ImGui::MenuItem("Layers", nullptr, prefs.is_layer_dock_visible())) {
      prefs.set_layer_dock_visible(!prefs.is_layer_dock_visible());
    }

    if (ImGui::MenuItem("Tilesets", nullptr, prefs.is_tileset_dock_visible())) {
      prefs.set_tileset_dock_visible(!prefs.is_tileset_dock_visible());
    }

    if (ImGui::MenuItem("Log", nullptr, prefs.is_log_dock_visible())) {
      prefs.set_log_dock_visible(!prefs.is_log_dock_visible());
    }

    if (ImGui::MenuItem("Components", nullptr, prefs.is_component_dock_visible())) {
      prefs.set_component_dock_visible(!prefs.is_component_dock_visible());
    }
  }
}

}  // namespace

void update_view_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (scoped::Menu menu{"View"}; menu.is_open()) {
    const auto hasActiveDocument = model.has_active_document();
    _update_widgets_menu(hasActiveDocument);

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_CENTER " Center Viewport",
                        "Shift+Space",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<CenterViewportEvent>();
    }

    ImGui::Separator();

    auto& prefs = get_preferences();

    if (bool visible = prefs.is_grid_visible();
        ImGui::MenuItem(TAC_ICON_GRID " Toggle Grid",
                        TACTILE_PRIMARY_MOD "+G",
                        &visible)) {
      prefs.set_grid_visible(visible);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_ZOOM_IN " Increase Zoom",
                        TACTILE_PRIMARY_MOD "+Plus",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<IncreaseZoomEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_ZOOM_OUT " Decrease Zoom",
                        TACTILE_PRIMARY_MOD "+Minus",
                        false,
                        model.can_decrease_viewport_tile_size())) {
      dispatcher.enqueue<DecreaseZoomEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_RESET_ZOOM " Reset Zoom",
                        nullptr,
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<ResetZoomEvent>();
    }

    ImGui::Separator();

    const auto fontSize = prefs.font_size();

    if (ImGui::MenuItem("Increase Font Size",
                        TACTILE_PRIMARY_MOD "+Shift+Plus",
                        false,
                        !prefs.use_default_font() && fontSize < get_max_font_size())) {
      dispatcher.enqueue<IncreaseFontSizeEvent>();
    }

    if (ImGui::MenuItem("Decrease Font Size",
                        TACTILE_PRIMARY_MOD "+Shift+Minus",
                        false,
                        !prefs.use_default_font() && fontSize > get_min_font_size())) {
      dispatcher.enqueue<DecreaseFontSizeEvent>();
    }

    if (ImGui::MenuItem("Reset Font Size", nullptr, false, !prefs.use_default_font())) {
      dispatcher.enqueue<ResetFontSizeEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_MOVE_UP " Pan Up",
                        TACTILE_PRIMARY_MOD "+Shift+Up",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<PanUpEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Pan Down",
                        TACTILE_PRIMARY_MOD "+Shift+Down",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<PanDownEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_RIGHT " Pan Right",
                        TACTILE_PRIMARY_MOD "+Shift+Right",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<PanRightEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_LEFT " Pan Left",
                        TACTILE_PRIMARY_MOD "+Shift+Left",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<PanLeftEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Highlight Active Layer",
                        "H",
                        prefs.highlight_active_layer(),
                        hasActiveDocument)) {
      prefs.set_highlight_active_layer(!prefs.highlight_active_layer());
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Toggle UI", "Tab", false, hasActiveDocument)) {
      dispatcher.enqueue<ToggleUiEvent>();
    }
  }
}

}  // namespace tactile
