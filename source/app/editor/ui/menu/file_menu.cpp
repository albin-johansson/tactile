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

#include "file_menu.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/util/filesystem.hpp"
#include "editor/handler/menu_event_handler.hpp"
#include "editor/shortcut/mappings.hpp"
#include "editor/ui/widget/menu_item.hpp"
#include "editor/ui/widget/scoped.hpp"
#include "io/proto/history.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/cmd/command_stack.hpp"
#include "model/event/map_events.hpp"
#include "model/model.hpp"

namespace tactile::ui {
namespace {

void update_recent_files_menu(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (Menu menu {lang.menu.recent_files.c_str()}; menu.is_open()) {
    menu_item(MenuAction::ReopenLastClosedFile);

    const auto& history = io::file_history();
    if (!history.empty()) {
      ImGui::Separator();
    }

    for (const auto& path: history) {
      if (ImGui::MenuItem(path.c_str())) {
        // It's fine if the file doesn't exist anymore, the parser handles that.
        dispatcher.enqueue<OpenMapEvent>(to_path(path));
      }
    }

    ImGui::Separator();

    menu_item(MenuAction::ClearFileHistory);
  }
}

}  // namespace

void update_file_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (Menu menu {lang.menu.file.c_str()}; menu.is_open()) {
    menu_item(MenuAction::NewMap, TACTILE_PRIMARY_MOD "+N");
    menu_item(MenuAction::OpenMap, TACTILE_PRIMARY_MOD "+O");

    update_recent_files_menu(dispatcher);

    ImGui::Separator();

    menu_item(MenuAction::Save, TACTILE_PRIMARY_MOD "+S");
    menu_item(MenuAction::SaveAs, TACTILE_PRIMARY_MOD "+Shift+S");

    ImGui::Separator();

    menu_item(MenuAction::CloseDocument);

    ImGui::Separator();

    menu_item(MenuAction::Quit);
  }
}

}  // namespace tactile::ui
