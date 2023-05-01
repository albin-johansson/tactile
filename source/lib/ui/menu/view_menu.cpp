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

#include "view_menu.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/viewport.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/model.hpp"
#include "model/settings.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/menu/menu.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

void update_widgets_menu(const DocumentModel& model)
{
  const auto& lang = get_current_language();

  if (const Menu menu {lang.menu.widgets.c_str(), model.has_active_document()};
      menu.is_open()) {
    if (ImGui::MenuItem(lang.action.reset_layout.c_str())) {
      reset_layout();
    }

    ImGui::Separator();

    auto& settings = get_settings();

    if (ImGui::MenuItem(lang.window.property_dock.c_str(),
                        nullptr,
                        settings.test_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT))) {
      settings.negate_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT);
    }

    {
      const Disable disable_unless_map {!model.is_map_active()};
      if (ImGui::MenuItem(lang.window.layer_dock.c_str(),
                          nullptr,
                          settings.test_flag(SETTINGS_SHOW_LAYER_DOCK_BIT))) {
        settings.negate_flag(SETTINGS_SHOW_LAYER_DOCK_BIT);
      }

      if (ImGui::MenuItem(lang.window.tileset_dock.c_str(),
                          nullptr,
                          settings.test_flag(SETTINGS_SHOW_TILESET_DOCK_BIT))) {
        settings.negate_flag(SETTINGS_SHOW_TILESET_DOCK_BIT);
      }
    }

    if (ImGui::MenuItem(lang.window.log_dock.c_str(),
                        nullptr,
                        settings.test_flag(SETTINGS_SHOW_LOG_DOCK_BIT))) {
      settings.negate_flag(SETTINGS_SHOW_LOG_DOCK_BIT);
    }

    if (ImGui::MenuItem(lang.window.component_dock.c_str(),
                        nullptr,
                        settings.test_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT))) {
      settings.negate_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT);
    }

    {
      const Disable disable_unless_tileset {!model.is_tileset_active()};
      if (ImGui::MenuItem(lang.window.animation_dock.c_str(),
                          nullptr,
                          settings.test_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT))) {
        settings.negate_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT);
      }
    }
  }
}

void update_quick_theme_menu(const Strings& lang)
{
  if (const Menu theme_menu {lang.action.quick_theme.c_str()}; theme_menu.is_open()) {
    auto& settings = get_settings();

    auto theme_item = [&](const EditorTheme theme) {
      const auto is_current = settings.get_theme() == theme;
      if (ImGui::MenuItem(human_readable_name(theme).data(), nullptr, is_current)) {
        settings.set_theme(theme);
        apply_theme(ImGui::GetStyle(), theme, settings.get_theme_saturation());
      }
    };

    for (const auto theme: kLightThemes) {
      theme_item(theme);
    }

    ImGui::Separator();

    for (const auto theme: kDarkThemes) {
      theme_item(theme);
    }
  }
}

void update_quick_lang_menu(const Strings& lang)
{
  if (const Menu lang_menu {lang.action.quick_language.c_str()}; lang_menu.is_open()) {
    auto& settings = get_settings();
    if (ImGui::MenuItem("English (US)")) {
      settings.set_language(Lang::EN);
      reset_layout();
      menu_translate(get_current_language());
    }

    if (ImGui::MenuItem("English (GB)")) {
      settings.set_language(Lang::EN_GB);
      reset_layout();
      menu_translate(get_current_language());
    }

    if (ImGui::MenuItem("Svenska")) {
      settings.set_language(Lang::SV);
      reset_layout();
      menu_translate(get_current_language());
    }
  }
}

}  // namespace

void update_view_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (const Menu menu {lang.menu.view.c_str()}; menu.is_open()) {
    update_widgets_menu(model);

    ImGui::Separator();
    update_quick_theme_menu(lang);

    ImGui::Separator();
    update_quick_lang_menu(lang);

    ImGui::Separator();
    ui_menu_item(dispatcher, MenuAction::CenterViewport, "Shift+Space");

    ImGui::Separator();
    ui_menu_item(dispatcher, MenuAction::ToggleGrid, TACTILE_PRIMARY_MOD "+G");

    ImGui::Separator();
    ui_menu_item(dispatcher, MenuAction::IncreaseZoom, TACTILE_PRIMARY_MOD "+Plus");
    ui_menu_item(dispatcher, MenuAction::DecreaseZoom, TACTILE_PRIMARY_MOD "+Minus");
    ui_menu_item(dispatcher, MenuAction::ResetZoom);

    ImGui::Separator();
    ui_menu_item(dispatcher,
                 MenuAction::IncreaseFontSize,
                 TACTILE_PRIMARY_MOD "+Shift+Plus");
    ui_menu_item(dispatcher,
                 MenuAction::DecreaseFontSize,
                 TACTILE_PRIMARY_MOD "+Shift+Minus");
    ui_menu_item(dispatcher, MenuAction::ResetFontSize);

    ImGui::Separator();
    ui_menu_item(dispatcher, MenuAction::PanUp, TACTILE_PRIMARY_MOD "+Shift+Up");
    ui_menu_item(dispatcher, MenuAction::PanDown, TACTILE_PRIMARY_MOD "+Shift+Down");
    ui_menu_item(dispatcher, MenuAction::PanRight, TACTILE_PRIMARY_MOD "+Shift+Right");
    ui_menu_item(dispatcher, MenuAction::PanLeft, TACTILE_PRIMARY_MOD "+Shift+Left");

    ImGui::Separator();
    ui_menu_item(dispatcher, MenuAction::HighlightLayer, "H");

    ImGui::Separator();
    ui_menu_item(dispatcher, MenuAction::ToggleUi, "Tab");
  }
}

}  // namespace tactile::ui
