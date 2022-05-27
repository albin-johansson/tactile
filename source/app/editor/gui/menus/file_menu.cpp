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

#include "editor/commands/command_stack.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/misc_events.hpp"
#include "editor/gui/dialogs/create_map_dialog.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "io/file_dialog.hpp"
#include "io/persistence/history.hpp"

namespace tactile {
namespace {

struct FileMenuState final
{
  CreateMapDialog map_creation_dialog;
  bool show_map_selector{};
};

[[nodiscard]] auto _get_state() -> FileMenuState&
{
  static FileMenuState state;
  return state;
}

void _update_map_file_dialog(entt::dispatcher& dispatcher)
{
  auto dialog = FileDialog::open_map();

  if (dialog.is_okay()) {
    dispatcher.enqueue<OpenMapEvent>(dialog.path());
  }

  _get_state().show_map_selector = false;
}

void _update_recent_files_menu(entt::dispatcher& dispatcher)
{
  if (scoped::Menu menu{TAC_ICON_HISTORY " Recent Files"}; menu.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_OPEN " Reopen Last Closed File",
                        nullptr,
                        false,
                        is_last_closed_file_valid())) {
      // TODO this will need to be tweaked if tileset documents viewing will be supported
      dispatcher.enqueue<OpenMapEvent>(last_closed_file());
    }

    const auto& history = file_history();

    if (!history.empty()) {
      ImGui::Separator();
    }

    for (const auto& path : history) {
      if (ImGui::MenuItem(path.c_str())) {
        /* It's fine if the file doesn't exist anymore, the parser handles that */
        dispatcher.enqueue<OpenMapEvent>(path);
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

}  // namespace

void update_file_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  auto& state = _get_state();

  if (scoped::Menu menu{"File"}; menu.is_open()) {
    const auto hasActiveDocument = model.has_active_document();
    const auto* document = model.active_document();

    if (ImGui::MenuItem(TAC_ICON_FILE " Create Map...", TACTILE_PRIMARY_MOD "+N")) {
      state.map_creation_dialog.show();
    }

    state.show_map_selector = ImGui::MenuItem(TAC_ICON_OPEN " Open Map...",  //
                                              TACTILE_PRIMARY_MOD "+O");

    _update_recent_files_menu(dispatcher);

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_SAVE " Save",
                        TACTILE_PRIMARY_MOD "+S",
                        false,
                        document && !document->get_history().is_clean())) {
      dispatcher.enqueue<SaveEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_SAVE " Save As...",
                        TACTILE_PRIMARY_MOD "+Shift+S",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<OpenSaveAsDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_CLOSE " Close Document",
                        nullptr,
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<CloseDocumentEvent>(model.active_document_id().value());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_EXIT " Exit")) {
      dispatcher.enqueue<QuitEvent>();
    }
  }

  state.map_creation_dialog.update(model, dispatcher);

  if (state.show_map_selector) {
    _update_map_file_dialog(dispatcher);
  }
}

void show_map_creation_dialog()
{
  _get_state().map_creation_dialog.show();
}

void show_map_selector_dialog()
{
  _get_state().show_map_selector = true;
}

}  // namespace tactile
