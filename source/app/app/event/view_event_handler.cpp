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

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include "app/app_context.hpp"
#include "app/event/event_handlers.hpp"
#include "common/debug/assert.hpp"
#include "model/document/map_document.hpp"
#include "model/event/view_events.hpp"
#include "model/model.hpp"
#include "model/settings.hpp"
#include "ui/fonts.hpp"
#include "ui/widget_show_state.hpp"

namespace tactile {
namespace {

void on_toggle_ui(const ToggleUiEvent&)
{
  spdlog::trace("ToggleUiEvent");

  if (ImGui::GetTopMostPopupModal() != nullptr) {
    return;
  }

  static bool show = false;

  auto& settings = get_settings();
  auto& state = get_widget_show_state();

  if (!show) {
    state.prev_show_layer_dock = settings.test_flag(SETTINGS_SHOW_LAYER_DOCK_BIT);
    state.prev_show_tileset_dock = settings.test_flag(SETTINGS_SHOW_TILESET_DOCK_BIT);
    state.prev_show_property_dock = settings.test_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT);
    state.prev_show_log_dock = settings.test_flag(SETTINGS_SHOW_LOG_DOCK_BIT);
    state.prev_show_component_dock = settings.test_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT);
    state.prev_show_animation_dock = settings.test_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT);
  }

  settings.set_flag(SETTINGS_SHOW_LAYER_DOCK_BIT, show);
  settings.set_flag(SETTINGS_SHOW_TILESET_DOCK_BIT, show);
  settings.set_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT, show);
  settings.set_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT, show);
  settings.set_flag(SETTINGS_SHOW_LOG_DOCK_BIT, show);
  settings.set_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT, show);

  if (show) {
    settings.set_flag(SETTINGS_SHOW_LAYER_DOCK_BIT, state.prev_show_layer_dock);
    settings.set_flag(SETTINGS_SHOW_TILESET_DOCK_BIT, state.prev_show_tileset_dock);
    settings.set_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT, state.prev_show_property_dock);
    settings.set_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT, state.prev_show_component_dock);
    settings.set_flag(SETTINGS_SHOW_LOG_DOCK_BIT, state.prev_show_log_dock);
    settings.set_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT, state.prev_show_animation_dock);
  }

  show = !show;
}

void on_reset_font_size(const ResetFontSizeEvent&)
{
  spdlog::trace("ResetFontSizeEvent");

  get_settings().set_font_size(ui::def_font_size);
  request_font_reload();
}

void on_increase_font_size(const IncreaseFontSizeEvent&)
{
  spdlog::trace("IncreaseFontSizeEvent");
  auto& settings = get_settings();

  TACTILE_ASSERT(settings.get_font_size() + 2 <= ui::max_font_size);
  settings.set_font_size(settings.get_font_size() + 2);

  request_font_reload();
}

void on_decrease_font_size(const DecreaseFontSizeEvent&)
{
  spdlog::trace("DecreaseFontSizeEvent");
  auto& settings = get_settings();

  TACTILE_ASSERT(settings.get_font_size() - 2 >= ui::min_font_size);
  settings.set_font_size(settings.get_font_size() - 2);

  request_font_reload();
}

void on_reload_fonts(const ReloadFontsEvent&)
{
  spdlog::trace("ReloadFontsEvent");
  request_font_reload();
}

}  // namespace

void install_view_event_handler()
{
  auto& dispatcher = get_dispatcher();
  dispatcher.sink<ToggleUiEvent>().connect<&on_toggle_ui>();

  dispatcher.sink<ResetFontSizeEvent>().connect<&on_reset_font_size>();
  dispatcher.sink<IncreaseFontSizeEvent>().connect<&on_increase_font_size>();
  dispatcher.sink<DecreaseFontSizeEvent>().connect<&on_decrease_font_size>();
  dispatcher.sink<ReloadFontsEvent>().connect<&on_reload_fonts>();
}

}  // namespace tactile
