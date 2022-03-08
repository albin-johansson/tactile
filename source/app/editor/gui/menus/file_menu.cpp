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

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/events/misc_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "io/file_dialog.hpp"
#include "io/persistence/history.hpp"

namespace tactile {

void FileMenu::Update(const document_model& model, entt::dispatcher& dispatcher)
{
  if (scoped::menu menu{"File"}; menu.is_open()) {
    const auto hasActiveDocument = model.has_active_document();

    if (ImGui::MenuItem(TAC_ICON_FILE " Create Map...", TACTILE_PRIMARY_MOD "+N")) {
      mCreateMapDialog.show();
    }

    mShowOpenMapDialog =
        ImGui::MenuItem(TAC_ICON_OPEN " Open Map...", TACTILE_PRIMARY_MOD "+O");

    UpdateRecentFilesMenu(dispatcher);

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_SAVE " Save",
                        TACTILE_PRIMARY_MOD "+S",
                        false,
                        model.is_save_possible())) {
      dispatcher.enqueue<save_event>();
    }

    if (ImGui::MenuItem(TAC_ICON_SAVE " Save As...",
                        TACTILE_PRIMARY_MOD "+Shift+S",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<open_save_as_dialog_event>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_CLOSE " Close Map", nullptr, false, hasActiveDocument)) {
      dispatcher.enqueue<close_map_event>(model.active_map_id().value());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_EXIT " Exit")) {
      dispatcher.enqueue<quit_event>();
    }
  }
}

void FileMenu::UpdateWindows(const document_model& model, entt::dispatcher& dispatcher)
{
  mCreateMapDialog.update(model, dispatcher);

  if (mShowOpenMapDialog) {
    UpdateMapFileDialog(dispatcher);
  }
}

void FileMenu::ShowNewMapDialog()
{
  mCreateMapDialog.show();
}

void FileMenu::ShowOpenMapDialog()
{
  mShowOpenMapDialog = true;
}

void FileMenu::UpdateRecentFilesMenu(entt::dispatcher& dispatcher)
{
  if (scoped::menu menu{TAC_ICON_HISTORY " Recent Files"}; menu.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_OPEN " Reopen Last Closed File",
                        nullptr,
                        false,
                        is_last_closed_file_valid())) {
      // TODO this will need to be tweaked if tileset documents viewing will be supported
      dispatcher.enqueue<open_map_event>(last_closed_file());
    }

    const auto& history = file_history();

    if (!history.empty()) {
      ImGui::Separator();
    }

    for (const auto& path : history) {
      if (ImGui::MenuItem(path.c_str())) {
        /* It's fine if the file doesn't exist anymore, the parser handles that */
        dispatcher.enqueue<open_map_event>(path);
      }
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_CLEAR_HISTORY " Clear File History",
                        nullptr,
                        false,
                        !history.empty())) {
      clear_file_history();
    }
  }
}

void FileMenu::UpdateMapFileDialog(entt::dispatcher& dispatcher)
{
  auto dialog = file_dialog::open_map();

  if (dialog.is_okay()) {
    dispatcher.enqueue<open_map_event>(dialog.path());
  }

  mShowOpenMapDialog = false;
}

}  // namespace tactile
