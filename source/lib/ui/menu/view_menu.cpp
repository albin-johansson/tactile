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

#include "model/events/menu_events.hpp"
#include "model/events/setting_events.hpp"
#include "model/events/view_events.hpp"
#include "model/i18n/language_system.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/style/themes.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile {
namespace {

void _push_widgets_menu(ModelView& model,
                        const Strings& strings,
                        const Settings& settings)
{
  if (const ui::Menu menu {strings.menu.widgets.c_str(), model.has_active_document()};
      menu.is_open()) {
    if (ImGui::MenuItem(strings.action.reset_layout.c_str())) {
      model.enqueue<ResetLayoutEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.window.property_dock.c_str(),
                        nullptr,
                        settings.test_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT))) {
      model.enqueue<NegateFlagSettingEvent>(SETTINGS_SHOW_PROPERTY_DOCK_BIT);
    }

    if (ImGui::MenuItem(strings.window.layer_dock.c_str(),
                        nullptr,
                        settings.test_flag(SETTINGS_SHOW_LAYER_DOCK_BIT),
                        model.has_active_map_document())) {
      model.enqueue<NegateFlagSettingEvent>(SETTINGS_SHOW_LAYER_DOCK_BIT);
    }

    if (ImGui::MenuItem(strings.window.tileset_dock.c_str(),
                        nullptr,
                        settings.test_flag(SETTINGS_SHOW_TILESET_DOCK_BIT),
                        model.has_active_map_document())) {
      model.enqueue<NegateFlagSettingEvent>(SETTINGS_SHOW_TILESET_DOCK_BIT);
    }

    if (ImGui::MenuItem(strings.window.log_dock.c_str(),
                        nullptr,
                        settings.test_flag(SETTINGS_SHOW_LOG_DOCK_BIT))) {
      model.enqueue<NegateFlagSettingEvent>(SETTINGS_SHOW_LOG_DOCK_BIT);
    }

    if (ImGui::MenuItem(strings.window.component_dock.c_str(),
                        nullptr,
                        settings.test_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT))) {
      model.enqueue<NegateFlagSettingEvent>(SETTINGS_SHOW_COMPONENT_DOCK_BIT);
    }

    if (ImGui::MenuItem(strings.window.animation_dock.c_str(),
                        nullptr,
                        settings.test_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT),
                        model.has_active_tileset_document())) {
      model.enqueue<NegateFlagSettingEvent>(SETTINGS_SHOW_ANIMATION_DOCK_BIT);
    }
  }
}

void _push_quick_theme_menu(ModelView& model,
                            const Strings& strings,
                            const Settings& settings)
{
  auto push_theme_item = [&](const Theme theme) {
    const auto is_current = settings.get_theme() == theme;
    if (ImGui::MenuItem(human_readable_name(theme).data(), nullptr, is_current)) {
      model.enqueue<SetThemeEvent>(theme);
    }
  };

  if (const ui::Menu theme_menu {strings.action.quick_theme.c_str()};
      theme_menu.is_open()) {
    for (const auto theme: kLightThemes) {
      push_theme_item(theme);
    }

    ImGui::Separator();

    for (const auto theme: kDarkThemes) {
      push_theme_item(theme);
    }
  }
}

void _push_quick_lang_menu(ModelView& model, const Strings& strings)
{
  if (const ui::Menu lang_menu {strings.action.quick_language.c_str()};
      lang_menu.is_open()) {
    if (ImGui::MenuItem("English (US)")) {
      model.enqueue<SetLanguageEvent>(Lang::EN);
    }

    if (ImGui::MenuItem("English (GB)")) {
      model.enqueue<SetLanguageEvent>(Lang::EN_GB);
    }

    if (ImGui::MenuItem("Svenska")) {
      model.enqueue<SetLanguageEvent>(Lang::SV);
    }
  }
}

}  // namespace

void push_view_menu(ModelView& model)
{
  const auto& strings = model.get_language_strings();
  const auto& settings = model.get_settings();

  if (const ui::Menu menu {strings.menu.view.c_str()}; menu.is_open()) {
    _push_widgets_menu(model, strings, settings);

    ImGui::Separator();

    _push_quick_theme_menu(model, strings, settings);

    ImGui::Separator();

    _push_quick_lang_menu(model, strings);

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.center_viewport.c_str(),
                        "Shift+Space",
                        nullptr,
                        model.is_available(MenuAction::CenterViewport))) {
      model.enqueue<MenuActionEvent>(MenuAction::CenterViewport);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.toggle_grid.c_str(),
                        TACTILE_PRIMARY_MOD "+G",
                        nullptr,
                        model.is_available(MenuAction::ToggleGrid))) {
      model.enqueue<MenuActionEvent>(MenuAction::ToggleGrid);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.increase_zoom.c_str(),
                        TACTILE_PRIMARY_MOD "+0",
                        nullptr,
                        model.is_available(MenuAction::IncreaseZoom))) {
      model.enqueue<MenuActionEvent>(MenuAction::IncreaseZoom);
    }

    if (ImGui::MenuItem(strings.action.decrease_zoom.c_str(),
                        TACTILE_PRIMARY_MOD "+9",
                        nullptr,
                        model.is_available(MenuAction::DecreaseZoom))) {
      model.enqueue<MenuActionEvent>(MenuAction::DecreaseZoom);
    }

    if (ImGui::MenuItem(strings.action.reset_zoom.c_str(),
                        nullptr,
                        nullptr,
                        model.is_available(MenuAction::ResetZoom))) {
      model.enqueue<MenuActionEvent>(MenuAction::ResetZoom);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.increase_font_size.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+0",
                        nullptr,
                        model.is_available(MenuAction::IncreaseFontSize))) {
      model.enqueue<MenuActionEvent>(MenuAction::IncreaseFontSize);
    }

    if (ImGui::MenuItem(strings.action.decrease_font_size.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+9",
                        nullptr,
                        model.is_available(MenuAction::DecreaseFontSize))) {
      model.enqueue<MenuActionEvent>(MenuAction::DecreaseFontSize);
    }

    if (ImGui::MenuItem(strings.action.reset_font_size.c_str(),
                        nullptr,
                        nullptr,
                        model.is_available(MenuAction::ResetFontSize))) {
      model.enqueue<MenuActionEvent>(MenuAction::ResetFontSize);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.pan_up.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+Up",
                        nullptr,
                        model.is_available(MenuAction::PanUp))) {
      model.enqueue<MenuActionEvent>(MenuAction::PanUp);
    }

    if (ImGui::MenuItem(strings.action.pan_down.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+Down",
                        nullptr,
                        model.is_available(MenuAction::PanDown))) {
      model.enqueue<MenuActionEvent>(MenuAction::PanDown);
    }

    if (ImGui::MenuItem(strings.action.pan_left.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+Left",
                        nullptr,
                        model.is_available(MenuAction::PanLeft))) {
      model.enqueue<MenuActionEvent>(MenuAction::PanLeft);
    }

    if (ImGui::MenuItem(strings.action.pan_right.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+Right",
                        nullptr,
                        model.is_available(MenuAction::PanRight))) {
      model.enqueue<MenuActionEvent>(MenuAction::PanRight);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.highlight_layer.c_str(),
                        "H",
                        nullptr,
                        model.is_available(MenuAction::HighlightLayer))) {
      model.enqueue<MenuActionEvent>(MenuAction::HighlightLayer);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.toggle_ui.c_str(),
                        "Tab",
                        nullptr,
                        model.is_available(MenuAction::ToggleUi))) {
      model.enqueue<MenuActionEvent>(MenuAction::ToggleUi);
    }
  }
}

}  // namespace tactile
