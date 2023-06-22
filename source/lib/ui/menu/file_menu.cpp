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

#include <imgui.h>

#include "common/type/path.hpp"
#include "model/events/map_events.hpp"
#include "model/events/menu_events.hpp"
#include "model/persistence/file_history_components.hpp"
#include "model/systems/language_system.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

void _push_recent_files_menu(const Model& model,
                             const Strings& strings,
                             Dispatcher& dispatcher)
{
  if (const Menu menu {strings.menu.recent_files.c_str()}; menu.is_open()) {
    push_menu_item(model, MenuAction::ReopenLastFile, dispatcher);

    const auto& history = model.get<FileHistory>();
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

    push_menu_item(model, MenuAction::ClearFileHistory, dispatcher);
  }
}

}  // namespace

void push_file_menu(const Model& model, Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  if (const Menu menu {strings.menu.file.c_str()}; menu.is_open()) {
    push_menu_item(model, MenuAction::NewMap, dispatcher);
    push_menu_item(model, MenuAction::OpenMap, dispatcher);

    _push_recent_files_menu(model, strings, dispatcher);

    ImGui::Separator();

    push_menu_item(model, MenuAction::Save, dispatcher);
    push_menu_item(model, MenuAction::SaveAs, dispatcher);

    ImGui::Separator();

    push_menu_item(model, MenuAction::Close, dispatcher);

    ImGui::Separator();

    push_menu_item(model, MenuAction::Quit, dispatcher);
  }
}

}  // namespace tactile::ui
