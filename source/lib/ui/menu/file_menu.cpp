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
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/map_events.hpp"
#include "model/event/menu_events.hpp"
#include "model/file_history.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

void _show_recent_files_menu(const Model& model,
                             const Strings& lang,
                             Dispatcher& dispatcher)
{
  if (const Menu menu {lang.menu.recent_files.c_str()}; menu.is_open()) {
    show_menu_item(model, MenuAction::ReopenLastFile, dispatcher);

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

    show_menu_item(model, MenuAction::ClearFileHistory, dispatcher);
  }
}

}  // namespace

void show_file_menu(const Model& model, Dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (const Menu menu {lang.menu.file.c_str()}; menu.is_open()) {
    show_menu_item(model, MenuAction::NewMap, dispatcher);
    show_menu_item(model, MenuAction::OpenMap, dispatcher);

    _show_recent_files_menu(model, lang, dispatcher);

    ImGui::Separator();

    show_menu_item(model, MenuAction::Save, dispatcher);
    show_menu_item(model, MenuAction::SaveAs, dispatcher);

    ImGui::Separator();

    show_menu_item(model, MenuAction::Close, dispatcher);

    ImGui::Separator();

    show_menu_item(model, MenuAction::Quit, dispatcher);
  }
}

}  // namespace tactile::ui
