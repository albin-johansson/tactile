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

#include <imgui.h>

#include "core/viewport.hpp"
#include "editor/shortcut/mappings.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/model.hpp"
#include "ui/app_context.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/widget/menu_item.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

void update_widgets_menu(const DocumentModel& model)
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
      Disable disable_if {!model.is_map_active()};
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

void update_view_menu()
{
  const auto& model = get_model();
  const auto& lang = get_current_language();

  if (Menu menu {lang.menu.view.c_str()}; menu.is_open()) {
    update_widgets_menu(model);

    ImGui::Separator();

    if (Menu theme_menu {lang.action.quick_theme.c_str()}; theme_menu.is_open()) {
      auto& prefs = io::get_preferences();

      auto theme_item = [&](const EditorTheme theme) {
        const auto is_current = prefs.theme == theme;
        if (ImGui::MenuItem(human_readable_name(theme).data(), nullptr, is_current)) {
          prefs.theme = theme;
          apply_theme(ImGui::GetStyle(), theme);
        }
      };

      for (const auto theme: light_themes) {
        theme_item(theme);
      }

      ImGui::Separator();

      for (const auto theme: dark_themes) {
        theme_item(theme);
      }
    }

    ImGui::Separator();

    if (Menu lang_menu {lang.action.quick_language.c_str()}; lang_menu.is_open()) {
      auto& prefs = io::get_preferences();
      if (ImGui::MenuItem("English (US)")) {
        prefs.language = Lang::EN;
        reset_layout();
      }

      if (ImGui::MenuItem("English (GB)")) {
        prefs.language = Lang::EN_GB;
        reset_layout();
      }

      if (ImGui::MenuItem("Svenska")) {
        prefs.language = Lang::SV;
        reset_layout();
      }
    }

    ImGui::Separator();

    menu_item(MenuAction::CenterViewport, "Shift+Space");

    ImGui::Separator();

    menu_item(MenuAction::ToggleGrid, TACTILE_PRIMARY_MOD "+G");

    ImGui::Separator();

    menu_item(MenuAction::IncreaseZoom, TACTILE_PRIMARY_MOD "+Plus");
    menu_item(MenuAction::DecreaseZoom, TACTILE_PRIMARY_MOD "+Minus");
    menu_item(MenuAction::ResetZoom);

    ImGui::Separator();

    menu_item(MenuAction::IncreaseFontSize, TACTILE_PRIMARY_MOD "+Shift+Plus");
    menu_item(MenuAction::DecreaseFontSize, TACTILE_PRIMARY_MOD "+Shift+Minus");
    menu_item(MenuAction::ResetFontSize);

    ImGui::Separator();

    menu_item(MenuAction::PanUp, TACTILE_PRIMARY_MOD "+Shift+Up");
    menu_item(MenuAction::PanDown, TACTILE_PRIMARY_MOD "+Shift+Down");
    menu_item(MenuAction::PanRight, TACTILE_PRIMARY_MOD "+Shift+Right");
    menu_item(MenuAction::PanLeft, TACTILE_PRIMARY_MOD "+Shift+Left");

    ImGui::Separator();

    menu_item(MenuAction::HighlightLayer, "H");

    ImGui::Separator();

    menu_item(MenuAction::ToggleUi, "Tab");
  }
}

}  // namespace tactile::ui
