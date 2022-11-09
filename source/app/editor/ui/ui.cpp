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

#include <spdlog/spdlog.h>

#include "core/type/path.hpp"
#include "editor/ui/dialog/dialog_state.hpp"
#include "editor/ui/dialog/dialogs.hpp"
#include "editor/ui/dock/comp/component_dock.hpp"
#include "editor/ui/dock/dock_space.hpp"
#include "editor/ui/dock/layer/layer_dock.hpp"
#include "editor/ui/dock/log/log_dock.hpp"
#include "editor/ui/dock/property/property_dock.hpp"
#include "editor/ui/dock/tileset/tileset_dock.hpp"
#include "editor/ui/menu/menu_bar.hpp"
#include "editor/ui/style/colors.hpp"
#include "editor/ui/viewport/map_viewport_toolbar.hpp"
#include "editor/ui/viewport/viewport_widget.hpp"
#include "io/directories.hpp"
#include "model/model.hpp"

namespace tactile::ui {
namespace {

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
  dialogs.resize_map.update(model, dispatcher);
  dialogs.map_parse_error.update(model, dispatcher);

  check_for_missing_ini_file();
}

auto is_editor_focused() -> bool
{
  return is_map_toolbar_focused() || is_viewport_focused() || is_layer_dock_focused() ||
         is_tileset_dock_focused() || is_property_dock_focused() || is_log_dock_focused();
}

}  // namespace tactile::ui