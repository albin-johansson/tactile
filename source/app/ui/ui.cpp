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

#include "ui.hpp"

#include <entt/signal/dispatcher.hpp>
#include <spdlog/spdlog.h>
#include <ui/style/alignment.hpp>

#include "core/predef.hpp"
#include "core/type/path.hpp"
#include "editor/app_context.hpp"
#include "io/directories.hpp"
#include "io/file_dialog.hpp"
#include "model/event/map_events.hpp"
#include "model/model.hpp"
#include "ui/dialog/dialog_state.hpp"
#include "ui/dialog/dialogs.hpp"
#include "ui/dock/comp/component_dock.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/dock/layer/layer_dock.hpp"
#include "ui/dock/log/log_dock.hpp"
#include "ui/dock/property/property_dock.hpp"
#include "ui/dock/tileset/tileset_dock.hpp"
#include "ui/menu/menu_bar.hpp"
#include "ui/style/colors.hpp"
#include "ui/viewport/map_viewport_toolbar.hpp"
#include "ui/viewport/viewport_widget.hpp"

namespace tactile::ui {
namespace {

inline constinit bool ui_show_map_selector = false;
inline constinit bool ui_show_about_imgui = false;

void check_for_missing_ini_file()
{
  const auto& ini = io::widget_ini_path();
  if (!fs::exists(ini)) {
    spdlog::info("Resetting layout because imgui.ini file was missing...");
    reset_layout();

    const auto str = ini.string();
    ImGui::SaveIniSettingsToDisk(str.c_str());
  }
}

void update_map_file_dialog(entt::dispatcher& dispatcher)
{
  auto dialog = io::FileDialog::open_map();

  if (dialog.is_okay()) {
    dispatcher.enqueue<OpenMapEvent>(dialog.path());
  }

  ui_show_map_selector = false;
}

}  // namespace

void update_widgets(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  update_dynamic_color_cache();

  update_menu_bar(model, dispatcher);
  update_dock_space();

  const auto* document = model.active_document();

  if (document) {
    if (document->is_map()) {
      update_layer_dock(model, dispatcher);
      update_tileset_dock(model, dispatcher);
    }

    update_property_dock(model, dispatcher);
    update_component_dock(model, dispatcher);
    update_log_dock();
  }

  update_viewport_widget(model, dispatcher);

  auto& dialogs = get_dialogs();
  dialogs.settings.update(model, dispatcher);
  dialogs.component_editor.update(model, dispatcher);
  dialogs.create_map.update(model, dispatcher);
  dialogs.resize_map.update(model, dispatcher);
  dialogs.map_parse_error.update(model, dispatcher);
  dialogs.about.update(model, dispatcher);
  dialogs.credits.update(model, dispatcher);
  dialogs.create_tileset.update(model, dispatcher);
  dialogs.godot_export.update(model, dispatcher);

  if (ui_show_map_selector) {
    update_map_file_dialog(dispatcher);
  }

  if (ui_show_about_imgui) {
    center_next_window_on_appearance();
    ImGui::ShowAboutWindow(&ui_show_about_imgui);
  }

  check_for_missing_ini_file();
}

void show_map_creation_dialog()
{
  get_dialogs().create_map.show();
}

void show_map_selector_dialog()
{
  ui_show_map_selector = true;
}

void show_about_dear_imgui_dialog()
{
  ui_show_about_imgui = true;
}

void show_settings_dialog()
{
  get_dialogs().settings.show();
}

void show_component_editor()
{
  const auto& model = get_model();
  get_dialogs().component_editor.show(model);
}

auto is_editor_focused() -> bool
{
  return is_map_toolbar_focused() || is_viewport_focused() || is_layer_dock_focused() ||
         is_tileset_dock_focused() || is_property_dock_focused() || is_log_dock_focused();
}

}  // namespace tactile::ui