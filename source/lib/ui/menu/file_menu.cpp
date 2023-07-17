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
#include "model/model_view.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile {
namespace {

void _push_recent_files_menu(ModelView& model)
{
  const auto& strings = model.get_language_strings();

  if (const ui::Menu menu {strings.menu.recent_files.c_str()}; menu.is_open()) {
    if (ImGui::MenuItem(strings.action.reopen_last_closed_file.c_str(),
                        nullptr,
                        false,
                        model.is_available(MenuAction::ReopenLastFile))) {
      model.enqueue<MenuActionEvent>(MenuAction::ReopenLastFile);
    }

    const auto& file_history = model.get_file_history();
    if (!file_history.empty()) {
      ImGui::Separator();
    }

    for (const auto& path: file_history) {
      if (ImGui::MenuItem(path.c_str())) {
        // It's fine if the file doesn't exist anymore, the parser handles that.
        model.enqueue<OpenMapEvent>(Path {path});
      }
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.clear_file_history.c_str(),
                        nullptr,
                        false,
                        model.is_available(MenuAction::ClearFileHistory))) {
      model.enqueue<MenuActionEvent>(MenuAction::ClearFileHistory);
    }
  }
}

}  // namespace

void push_file_menu(ModelView& model)
{
  const auto& strings = model.get_language_strings();

  if (const ui::Menu menu {strings.menu.file.c_str()}; menu.is_open()) {
    if (ImGui::MenuItem(strings.action.create_map.c_str(),
                        TACTILE_PRIMARY_MOD "+N",
                        nullptr,
                        model.is_available(MenuAction::NewMap))) {
      model.enqueue<MenuActionEvent>(MenuAction::NewMap);
    }

    if (ImGui::MenuItem(strings.action.open_map.c_str(),
                        TACTILE_PRIMARY_MOD "+O",
                        nullptr,
                        model.is_available(MenuAction::OpenMap))) {
      model.enqueue<MenuActionEvent>(MenuAction::OpenMap);
    }

    _push_recent_files_menu(model);

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.save.c_str(),
                        TACTILE_PRIMARY_MOD "+S",
                        false,
                        model.is_available(MenuAction::Save))) {
      model.enqueue<MenuActionEvent>(MenuAction::Save);
    }

    if (ImGui::MenuItem(strings.action.save_as.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+S",
                        false,
                        model.is_available(MenuAction::SaveAs))) {
      model.enqueue<MenuActionEvent>(MenuAction::SaveAs);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.close_document.c_str(),
                        nullptr,
                        false,
                        model.is_available(MenuAction::Close))) {
      model.enqueue<MenuActionEvent>(MenuAction::Close);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.exit.c_str(),
                        nullptr,
                        false,
                        model.is_available(MenuAction::Quit))) {
      model.enqueue<MenuActionEvent>(MenuAction::Quit);
    }
  }
}

}  // namespace tactile
