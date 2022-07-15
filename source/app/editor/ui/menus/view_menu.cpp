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

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/event/misc_events.hpp"
#include "core/event/viewport_events.hpp"
#include "core/model.hpp"
#include "core/viewport.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "editor/ui/dock_space.hpp"
#include "editor/ui/fonts.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile::ui {
namespace {

void _update_widgets_menu(const DocumentModel& model)
{
  if (Menu menu {"Widgets", model.has_active_document()}; menu.is_open()) {
    if (ImGui::MenuItem("Reset Layout")) {
      reset_layout();
    }

    ImGui::Separator();

    auto& prefs = io::get_preferences();

    if (ImGui::MenuItem("Properties", nullptr, prefs.show_property_dock)) {
      prefs.show_property_dock = !prefs.show_property_dock;
    }

    {
      Disable disableIf {!model.is_map_active()};
      if (ImGui::MenuItem("Layers", nullptr, prefs.show_layer_dock)) {
        prefs.show_layer_dock = !prefs.show_layer_dock;
      }

      if (ImGui::MenuItem("Tilesets", nullptr, prefs.show_tileset_dock)) {
        prefs.show_tileset_dock = !prefs.show_tileset_dock;
      }
    }

    if (ImGui::MenuItem("Log", nullptr, prefs.show_log_dock)) {
      prefs.show_log_dock = !prefs.show_log_dock;
    }

    if (ImGui::MenuItem("Components", nullptr, prefs.show_component_dock)) {
      prefs.show_component_dock = !prefs.show_component_dock;
    }
  }
}

}  // namespace

void update_view_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (Menu menu {"View"}; menu.is_open()) {
    auto&       prefs = io::get_preferences();
    const auto* document = model.active_document();
    const auto  hasActiveDocument = model.has_active_document();

    _update_widgets_menu(model);

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_CENTER " Center Viewport",
                        "Shift+Space",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<CenterViewportEvent>();
    }

    ImGui::Separator();

    ImGui::MenuItem(TAC_ICON_GRID " Toggle Grid",
                    TACTILE_PRIMARY_MOD "+G",
                    &prefs.show_grid);

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
                        document && document->get_viewport().can_zoom_out())) {
      dispatcher.enqueue<DecreaseZoomEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_RESET_ZOOM " Reset Zoom",
                        nullptr,
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<ResetZoomEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Increase Font Size",
                        TACTILE_PRIMARY_MOD "+Shift+Plus",
                        false,
                        can_increase_font_size())) {
      dispatcher.enqueue<IncreaseFontSizeEvent>();
    }

    if (ImGui::MenuItem("Decrease Font Size",
                        TACTILE_PRIMARY_MOD "+Shift+Minus",
                        false,
                        can_decrease_font_size())) {
      dispatcher.enqueue<DecreaseFontSizeEvent>();
    }

    if (ImGui::MenuItem("Reset Font Size", nullptr, false, !prefs.use_default_font)) {
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

    ImGui::MenuItem("Highlight Active Layer",
                    "H",
                    &prefs.highlight_active_layer,
                    model.is_map_active());

    ImGui::Separator();

    if (ImGui::MenuItem("Toggle UI", "Tab", false, hasActiveDocument)) {
      dispatcher.enqueue<ToggleUiEvent>();
    }
  }
}

}  // namespace tactile::ui
