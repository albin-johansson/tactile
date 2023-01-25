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

#include "file_menu.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "common/util/filesystem.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/map_events.hpp"
#include "model/file_history.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

void update_recent_files_menu(const Strings& lang, entt::dispatcher& dispatcher)
{
  if (const Menu menu {lang.menu.recent_files.c_str()}; menu.is_open()) {
    ui_menu_item(dispatcher, MenuAction::ReopenLastClosedFile);

    const auto& history = get_file_history();
    if (!history.entries.empty()) {
      ImGui::Separator();
    }

    for (const auto& path: history.entries) {
      if (ImGui::MenuItem(path.c_str())) {
        // It's fine if the file doesn't exist anymore, the parser handles that.
        dispatcher.enqueue<OpenMapEvent>(Path {path});
      }
    }

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::ClearFileHistory);
  }
}

}  // namespace

void update_file_menu(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (const Menu menu {lang.menu.file.c_str()}; menu.is_open()) {
    ui_menu_item(dispatcher, MenuAction::NewMap, TACTILE_PRIMARY_MOD "+N");
    ui_menu_item(dispatcher, MenuAction::OpenMap, TACTILE_PRIMARY_MOD "+O");

    update_recent_files_menu(lang, dispatcher);

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::Save, TACTILE_PRIMARY_MOD "+S");
    ui_menu_item(dispatcher, MenuAction::SaveAs, TACTILE_PRIMARY_MOD "+Shift+S");

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::CloseDocument);

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::Quit);
  }
}

}  // namespace tactile::ui
