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
#include "editor/shortcut/mappings.hpp"
#include "editor/ui/dialog/dialog_state.hpp"
#include "editor/ui/dialog/dialogs.hpp"
#include "editor/ui/widget/scoped.hpp"
#include "io/file_dialog.hpp"
#include "io/proto/history.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/cmd/command_stack.hpp"
#include "model/event/document_events.hpp"
#include "model/event/map_events.hpp"
#include "model/event/view_events.hpp"
#include "model/model.hpp"

namespace tactile::ui {
namespace {

inline constinit bool menu_show_map_selector = false;

void update_map_file_dialog(entt::dispatcher& dispatcher)
{
  auto dialog = io::FileDialog::open_map();

  if (dialog.is_okay()) {
    dispatcher.enqueue<OpenMapEvent>(dialog.path());
  }

  menu_show_map_selector = false;
}

void update_recent_files_menu(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (Menu menu {lang.menu.recent_files.c_str()}; menu.is_open()) {
    if (ImGui::MenuItem(lang.action.reopen_last_closed_file.c_str(),
                        nullptr,
                        false,
                        io::is_last_closed_file_valid())) {
      // TODO this will need to be tweaked if tileset documents viewing will be supported
      dispatcher.enqueue<OpenMapEvent>(to_path(io::last_closed_file()));
    }

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

    if (ImGui::MenuItem(lang.action.clear_file_history.c_str(),
                        nullptr,
                        false,
                        !history.empty())) {
      io::clear_file_history();
    }
  }
}

}  // namespace

void update_file_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (Menu menu {lang.menu.file.c_str()}; menu.is_open()) {
    const auto has_active_document = model.has_active_document();
    const auto* document = model.active_document();

    if (ImGui::MenuItem(lang.action.create_map.c_str(), TACTILE_PRIMARY_MOD "+N")) {
      get_dialogs().create_map.show();
    }

    menu_show_map_selector =
        ImGui::MenuItem(lang.action.open_map.c_str(), TACTILE_PRIMARY_MOD "+O");

    update_recent_files_menu(dispatcher);

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.save.c_str(),
                        TACTILE_PRIMARY_MOD "+S",
                        false,
                        document && !document->get_history().is_clean())) {
      dispatcher.enqueue<SaveEvent>();
    }

    if (ImGui::MenuItem(lang.action.save_as.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+S",
                        false,
                        has_active_document)) {
      dispatcher.enqueue<OpenSaveAsDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.close_document.c_str(),
                        nullptr,
                        false,
                        has_active_document)) {
      dispatcher.enqueue<CloseDocumentEvent>(model.active_document_id().value());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.exit.c_str())) {
      dispatcher.enqueue<QuitEvent>();
    }
  }

  get_dialogs().create_map.update(model, dispatcher);

  if (menu_show_map_selector) {
    update_map_file_dialog(dispatcher);
  }
}

void show_map_creation_dialog()
{
  get_dialogs().create_map.show();
}

void show_map_selector_dialog()
{
  menu_show_map_selector = true;
}

}  // namespace tactile::ui
