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
#include "editor/lang/language.hpp"
#include "editor/lang/strings.hpp"
#include "editor/shortcut/mappings.hpp"
#include "editor/ui/dock_space.hpp"
#include "editor/ui/fonts.hpp"
#include "editor/ui/scoped.hpp"
#include "io/persist/preferences.hpp"

namespace tactile::ui {
namespace {

void _update_widgets_menu(const DocumentModel& model)
{
  const auto& lang = get_current_language();

  if (Menu menu {lang.menu.widgets.c_str(), model.has_active_document()};
      menu.is_open()) {
    if (ImGui::MenuItem(lang.action.reset_layout.c_str())) {
      reset_layout();
    }

    ImGui::Separator();

    auto& prefs = io::get_preferences();

    if (ImGui::MenuItem(lang.window.property_dock.c_str(),
                        nullptr,
                        prefs.show_property_dock)) {
      prefs.show_property_dock = !prefs.show_property_dock;
    }

    {
      Disable disableIf {!model.is_map_active()};
      if (ImGui::MenuItem(lang.window.layer_dock.c_str(),
                          nullptr,
                          prefs.show_layer_dock)) {
        prefs.show_layer_dock = !prefs.show_layer_dock;
      }

      if (ImGui::MenuItem(lang.window.tileset_dock.c_str(),
                          nullptr,
                          prefs.show_tileset_dock)) {
        prefs.show_tileset_dock = !prefs.show_tileset_dock;
      }
    }

    if (ImGui::MenuItem(lang.window.log_dock.c_str(), nullptr, prefs.show_log_dock)) {
      prefs.show_log_dock = !prefs.show_log_dock;
    }

    if (ImGui::MenuItem(lang.window.component_dock.c_str(),
                        nullptr,
                        prefs.show_component_dock)) {
      prefs.show_component_dock = !prefs.show_component_dock;
    }
  }
}

}  // namespace

void update_view_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (Menu menu {lang.menu.view.c_str()}; menu.is_open()) {
    auto&       prefs = io::get_preferences();
    const auto* document = model.active_document();
    const auto  hasActiveDocument = model.has_active_document();

    _update_widgets_menu(model);

    ImGui::Separator();

    if (Menu themeMenu {lang.action.quick_theme.c_str()}; themeMenu.is_open()) {
      auto themeItem = [&](const EditorTheme theme) {
        const auto isCurrent = prefs.theme == theme;
        if (ImGui::MenuItem(human_readable_name(theme).data(), nullptr, isCurrent)) {
          prefs.theme = theme;
          apply_theme(ImGui::GetStyle(), theme);
        }
      };

      for (const auto theme : light_themes) {
        themeItem(theme);
      }

      ImGui::Separator();

      for (const auto theme : dark_themes) {
        themeItem(theme);
      }
    }

    ImGui::Separator();

    if (Menu langMenu {lang.action.quick_language.c_str()}; langMenu.is_open()) {
      if (ImGui::MenuItem("English (US)")) {
        prefs.language = Lang::EN;
      }

      if (ImGui::MenuItem("English (GB)")) {
        prefs.language = Lang::EN_GB;
      }

      if (ImGui::MenuItem("Svenska")) {
        prefs.language = Lang::SV;
      }
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.center_viewport.c_str(),
                        "Shift+Space",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<CenterViewportEvent>();
    }

    ImGui::Separator();

    ImGui::MenuItem(lang.action.toggle_grid.c_str(),
                    TACTILE_PRIMARY_MOD "+G",
                    &prefs.show_grid);

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.increase_zoom.c_str(),
                        TACTILE_PRIMARY_MOD "+Plus",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<IncreaseZoomEvent>();
    }

    if (ImGui::MenuItem(lang.action.decrease_zoom.c_str(),
                        TACTILE_PRIMARY_MOD "+Minus",
                        false,
                        document && document->get_viewport().can_zoom_out())) {
      dispatcher.enqueue<DecreaseZoomEvent>();
    }

    if (ImGui::MenuItem(lang.action.reset_zoom.c_str(),
                        nullptr,
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<ResetZoomEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.increase_font_size.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+Plus",
                        false,
                        can_increase_font_size())) {
      dispatcher.enqueue<IncreaseFontSizeEvent>();
    }

    if (ImGui::MenuItem(lang.action.decrease_font_size.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+Minus",
                        false,
                        can_decrease_font_size())) {
      dispatcher.enqueue<DecreaseFontSizeEvent>();
    }

    if (ImGui::MenuItem(lang.action.reset_font_size.c_str(),
                        nullptr,
                        false,
                        !prefs.use_default_font)) {
      dispatcher.enqueue<ResetFontSizeEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.pan_up.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+Up",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<PanUpEvent>();
    }

    if (ImGui::MenuItem(lang.action.pan_down.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+Down",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<PanDownEvent>();
    }

    if (ImGui::MenuItem(lang.action.pan_right.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+Right",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<PanRightEvent>();
    }

    if (ImGui::MenuItem(lang.action.pan_left.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+Left",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<PanLeftEvent>();
    }

    ImGui::Separator();

    ImGui::MenuItem(lang.action.highlight_layer.c_str(),
                    "H",
                    &prefs.highlight_active_layer,
                    model.is_map_active());

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.toggle_ui.c_str(), "Tab", false, hasActiveDocument)) {
      dispatcher.enqueue<ToggleUiEvent>();
    }
  }
}

}  // namespace tactile::ui
