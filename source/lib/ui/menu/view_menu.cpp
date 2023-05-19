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

#include <imgui.h>

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/context.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/menu_system.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

void _show_widgets_menu(const Model& model)
{
  const auto& lang = get_current_language();

  if (const Menu menu {lang.menu.widgets.c_str(), sys::has_active_document(model)};
      menu.is_open()) {
    if (ImGui::MenuItem(lang.action.reset_layout.c_str())) {
      reset_layout();
    }

    ImGui::Separator();

    auto& settings = get_global_settings();

    if (ImGui::MenuItem(lang.window.property_dock.c_str(),
                        nullptr,
                        settings.test_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT))) {
      settings.negate_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT);
    }

    {
      const Disable disable_if {!sys::is_map_document_active(model)};
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
      const Disable disable_if {!sys::is_tileset_document_active(model)};
      if (ImGui::MenuItem(lang.window.animation_dock.c_str(),
                          nullptr,
                          settings.test_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT))) {
        settings.negate_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT);
      }
    }
  }
}

void _show_quick_theme_menu(const Strings& lang)
{
  if (const Menu theme_menu {lang.action.quick_theme.c_str()}; theme_menu.is_open()) {
    auto& settings = get_global_settings();

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

void _show_quick_lang_menu(const Strings& lang)
{
  if (const Menu lang_menu {lang.action.quick_language.c_str()}; lang_menu.is_open()) {
    auto& settings = get_global_settings();
    if (ImGui::MenuItem("English (US)")) {
      // TODO enqueue event
      // settings.set_language(Lang::EN);
      // reset_layout();
      // menu_translate(get_current_language());
    }

    if (ImGui::MenuItem("English (GB)")) {
      // TODO enqueue event
      // settings.set_language(Lang::EN_GB);
      // reset_layout();
      // menu_translate(get_current_language());
    }

    if (ImGui::MenuItem("Svenska")) {
      // TODO enqueue event
      // settings.set_language(Lang::SV);
      // reset_layout();
      // menu_translate(get_current_language());
    }
  }
}

}  // namespace

void show_view_menu(const Model& model, Dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (const Menu menu {lang.menu.view.c_str()}; menu.is_open()) {
    _show_widgets_menu(model);

    ImGui::Separator();
    _show_quick_theme_menu(lang);

    ImGui::Separator();
    _show_quick_lang_menu(lang);

    ImGui::Separator();
    show_menu_item(model, MenuAction::CenterViewport, dispatcher);

    ImGui::Separator();
    show_menu_item(model, MenuAction::ToggleGrid, dispatcher);

    ImGui::Separator();
    show_menu_item(model, MenuAction::IncreaseZoom, dispatcher);
    show_menu_item(model, MenuAction::DecreaseZoom, dispatcher);
    show_menu_item(model, MenuAction::ResetZoom, dispatcher);

    ImGui::Separator();
    show_menu_item(model, MenuAction::IncreaseFontSize, dispatcher);
    show_menu_item(model, MenuAction::DecreaseFontSize, dispatcher);
    show_menu_item(model, MenuAction::ResetFontSize, dispatcher);

    ImGui::Separator();
    show_menu_item(model, MenuAction::PanUp, dispatcher);
    show_menu_item(model, MenuAction::PanDown, dispatcher);
    show_menu_item(model, MenuAction::PanRight, dispatcher);
    show_menu_item(model, MenuAction::PanLeft, dispatcher);

    ImGui::Separator();
    show_menu_item(model, MenuAction::HighlightLayer, dispatcher);

    ImGui::Separator();
    show_menu_item(model, MenuAction::ToggleUi, dispatcher);
  }
}

}  // namespace tactile::ui
