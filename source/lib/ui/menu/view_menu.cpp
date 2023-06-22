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

#include "model/documents/document_system.hpp"
#include "model/event/setting_events.hpp"
#include "model/event/view_events.hpp"
#include "model/systems/language_system.hpp"
#include "ui/style/themes.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

void _push_widgets_menu(const Model& model,
                        const Strings& strings,
                        Dispatcher& dispatcher)
{
  if (const Menu menu {strings.menu.widgets.c_str(), sys::has_active_document(model)};
      menu.is_open()) {
    if (ImGui::MenuItem(strings.action.reset_layout.c_str())) {
      dispatcher.enqueue<ResetLayoutEvent>();
    }

    ImGui::Separator();

    const auto& settings = model.get<Settings>();

    if (ImGui::MenuItem(strings.window.property_dock.c_str(),
                        nullptr,
                        settings.test_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT))) {
      dispatcher.enqueue<NegateFlagSettingEvent>(SETTINGS_SHOW_PROPERTY_DOCK_BIT);
    }

    {
      const Disable disable_if {!sys::is_map_document_active(model)};
      if (ImGui::MenuItem(strings.window.layer_dock.c_str(),
                          nullptr,
                          settings.test_flag(SETTINGS_SHOW_LAYER_DOCK_BIT))) {
        dispatcher.enqueue<NegateFlagSettingEvent>(SETTINGS_SHOW_LAYER_DOCK_BIT);
      }

      if (ImGui::MenuItem(strings.window.tileset_dock.c_str(),
                          nullptr,
                          settings.test_flag(SETTINGS_SHOW_TILESET_DOCK_BIT))) {
        dispatcher.enqueue<NegateFlagSettingEvent>(SETTINGS_SHOW_TILESET_DOCK_BIT);
      }
    }

    if (ImGui::MenuItem(strings.window.log_dock.c_str(),
                        nullptr,
                        settings.test_flag(SETTINGS_SHOW_LOG_DOCK_BIT))) {
      dispatcher.enqueue<NegateFlagSettingEvent>(SETTINGS_SHOW_LOG_DOCK_BIT);
    }

    if (ImGui::MenuItem(strings.window.component_dock.c_str(),
                        nullptr,
                        settings.test_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT))) {
      dispatcher.enqueue<NegateFlagSettingEvent>(SETTINGS_SHOW_COMPONENT_DOCK_BIT);
    }

    {
      const Disable disable_if {!sys::is_tileset_document_active(model)};
      if (ImGui::MenuItem(strings.window.animation_dock.c_str(),
                          nullptr,
                          settings.test_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT))) {
        dispatcher.enqueue<NegateFlagSettingEvent>(SETTINGS_SHOW_ANIMATION_DOCK_BIT);
      }
    }
  }
}

void _push_quick_theme_menu(const Settings& settings,
                            const Strings& strings,
                            Dispatcher& dispatcher)
{
  if (const Menu theme_menu {strings.action.quick_theme.c_str()}; theme_menu.is_open()) {
    auto theme_item = [&](const Theme theme) {
      const auto is_current = settings.get_theme() == theme;
      if (ImGui::MenuItem(human_readable_name(theme).data(), nullptr, is_current)) {
        dispatcher.enqueue<SetThemeEvent>(theme);
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

void _push_quick_lang_menu(const Strings& strings, Dispatcher& dispatcher)
{
  if (const Menu lang_menu {strings.action.quick_language.c_str()}; lang_menu.is_open()) {
    if (ImGui::MenuItem("English (US)")) {
      dispatcher.enqueue<SetLanguageEvent>(Lang::EN);
    }

    if (ImGui::MenuItem("English (GB)")) {
      dispatcher.enqueue<SetLanguageEvent>(Lang::EN_GB);
    }

    if (ImGui::MenuItem("Svenska")) {
      dispatcher.enqueue<SetLanguageEvent>(Lang::SV);
    }
  }
}

}  // namespace

void push_view_menu(const Model& model, Dispatcher& dispatcher)
{
  const auto& settings = model.get<Settings>();
  const auto& strings = sys::get_current_language_strings(model);

  if (const Menu menu {strings.menu.view.c_str()}; menu.is_open()) {
    _push_widgets_menu(model, strings, dispatcher);

    ImGui::Separator();
    _push_quick_theme_menu(settings, strings, dispatcher);

    ImGui::Separator();
    _push_quick_lang_menu(strings, dispatcher);

    ImGui::Separator();
    push_menu_item(model, MenuAction::CenterViewport, dispatcher);

    ImGui::Separator();
    push_menu_item(model, MenuAction::ToggleGrid, dispatcher);

    ImGui::Separator();
    push_menu_item(model, MenuAction::IncreaseZoom, dispatcher);
    push_menu_item(model, MenuAction::DecreaseZoom, dispatcher);
    push_menu_item(model, MenuAction::ResetZoom, dispatcher);

    ImGui::Separator();
    push_menu_item(model, MenuAction::IncreaseFontSize, dispatcher);
    push_menu_item(model, MenuAction::DecreaseFontSize, dispatcher);
    push_menu_item(model, MenuAction::ResetFontSize, dispatcher);

    ImGui::Separator();
    push_menu_item(model, MenuAction::PanUp, dispatcher);
    push_menu_item(model, MenuAction::PanDown, dispatcher);
    push_menu_item(model, MenuAction::PanRight, dispatcher);
    push_menu_item(model, MenuAction::PanLeft, dispatcher);

    ImGui::Separator();
    push_menu_item(model, MenuAction::HighlightLayer, dispatcher);

    ImGui::Separator();
    push_menu_item(model, MenuAction::ToggleUi, dispatcher);
  }
}

}  // namespace tactile::ui
