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

#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

#include "model/event/command_events.hpp"
#include "model/event/view_events.hpp"
#include "model/settings.hpp"
#include "model/systems/menu_system.hpp"
#include "ui/dialog/settings_dialog.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/style/themes.hpp"

namespace tactile {

void on_show_settings(Model& model, const ShowSettingsEvent&)
{
  spdlog::trace("[ShowSettingsEvent]");

  const auto& settings = model.get<Settings>();
  ui::open_settings_dialog(settings);
}

void on_set_settings(Model& model, Dispatcher& dispatcher, const SetSettingsEvent& event)
{
  spdlog::trace("[SetSettingsEvent]");

  auto& curr_settings = model.get<Settings>();

  const auto prev_settings = curr_settings.copy();
  curr_settings = event.settings.copy();

  if (curr_settings.get_language() != prev_settings.get_language()) {
    sys::retranslate_menus(model);
    ui::reset_layout(model, dispatcher);
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

void on_set_language(Model& model, Dispatcher& dispatcher, const SetLanguageEvent& event)
{
  spdlog::trace("[SetLanguageEvent] language: {}", magic_enum::enum_name(event.language));

  auto& settings = model.get<Settings>();
  settings.set_language(event.language);

  sys::retranslate_menus(model);
  ui::reset_layout(model, dispatcher);
}

void on_set_theme(Model& model, const SetThemeEvent& event)
{
  auto& settings = model.get<Settings>();
  settings.set_theme(event.theme);

  ui::apply_theme(ImGui::GetStyle(),
                  settings.get_theme(),
                  settings.get_theme_saturation());
}

void on_reset_dock_visibilities(Model& model, const ResetDockVisibilitiesEvent& event)
{
  auto& settings = model.get<Settings>();
  settings.reset_dock_visibilities();
}

}  // namespace tactile
