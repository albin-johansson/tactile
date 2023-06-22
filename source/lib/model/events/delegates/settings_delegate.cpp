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

#include "settings_delegate.hpp"

#include "model/events/command_events.hpp"
#include "model/events/font_events.hpp"
#include "model/events/view_events.hpp"
#include "model/menus/menu_system.hpp"
#include "model/persistence/settings.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/style/themes.hpp"
#include "ui/widget_state.hpp"

namespace tactile {

void on_show_settings(Model& model, const ShowSettingsEvent&)
{
  const auto& settings = model.get<Settings>();

  auto& widgets = model.get<ui::WidgetState>();
  widgets.settings_dialog.old_settings.copy_from(settings);
  widgets.settings_dialog.ui_settings.copy_from(settings);
  widgets.settings_dialog.should_open = true;
}

void on_set_settings(Model& model, Dispatcher& dispatcher, const SetSettingsEvent& event)
{
  auto& curr_settings = model.get<Settings>();

  const auto prev_settings = curr_settings.copy();
  curr_settings = event.settings.copy();

  if (curr_settings.get_language() != prev_settings.get_language()) {
    sys::retranslate_menus(model);
    dispatcher.enqueue<ResetLayoutEvent>();
  }

  if (curr_settings.get_command_capacity() != prev_settings.get_command_capacity()) {
    dispatcher.trigger(SetCommandCapacityEvent {curr_settings.get_command_capacity()});
  }

  if (curr_settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT) !=
          prev_settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT) ||
      curr_settings.get_font_size() != prev_settings.get_font_size()) {
    dispatcher.trigger(ReloadFontsEvent {});
  }
}

void on_set_flag_setting(Model& model, const SetFlagSettingEvent& event)
{
  auto& settings = model.get<Settings>();
  settings.set_flag(event.flag, event.value);
}

void on_negate_flag_setting(Model& model, const NegateFlagSettingEvent& event)
{
  auto& settings = model.get<Settings>();
  settings.negate_flag(event.flag);
}

void on_set_viewport_overlay_pos(Model& model, const SetViewportOverlayPosEvent& event)
{
  auto& settings = model.get<Settings>();
  settings.set_viewport_overlay_pos(event.pos);
}

void on_set_language(Model& model, Dispatcher& dispatcher, const SetLanguageEvent& event)
{
  auto& settings = model.get<Settings>();
  settings.set_language(event.language);

  sys::retranslate_menus(model);
  dispatcher.enqueue<ResetLayoutEvent>();
}

void on_set_theme(Model& model, const SetThemeEvent& event)
{
  auto& settings = model.get<Settings>();
  settings.set_theme(event.theme);

  ui::apply_theme(ImGui::GetStyle(),
                  settings.get_theme(),
                  settings.get_theme_saturation());
}

void on_reset_dock_visibilities(Model& model, const ResetDockVisibilitiesEvent&)
{
  auto& settings = model.get<Settings>();
  settings.reset_dock_visibilities();
}

}  // namespace tactile
