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

#include "menu_bar.hpp"

#include <entt/entt.hpp>
#include <imgui.h>

#include "debug_menu.hpp"
#include "editor/model.hpp"
#include "view_menu.hpp"

namespace tactile {

void MenuBar::update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMainMenuBar()) {
    mFileMenu.update(model, dispatcher);
    mEditMenu.update(model, dispatcher);
    update_view_menu(model, dispatcher);
    mMapMenu.update(model, dispatcher);
    mHelpMenu.update(model, dispatcher);
    update_debug_menu();

    ImGui::EndMainMenuBar();
  }

  mFileMenu.update_windows(model, dispatcher);
  mEditMenu.update_windows(model, dispatcher);
  mHelpMenu.update_windows();
  update_debug_menu_windows();
}

void MenuBar::show_settings_dialog()
{
  mEditMenu.show_settings_dialog();
}

void MenuBar::show_map_creation_dialog()
{
  mFileMenu.show_map_creation_dialog();
}

void MenuBar::show_open_map_dialog()
{
  mFileMenu.show_open_map_dialog();
}

void MenuBar::show_tileset_creation_dialog()
{
  mMapMenu.show_tileset_creation_dialog();
}

void MenuBar::show_component_editor(const DocumentModel& model)
{
  mEditMenu.show_component_editor(model);
}

}  // namespace tactile
