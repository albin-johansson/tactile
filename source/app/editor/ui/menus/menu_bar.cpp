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

#include <imgui.h>

#include "core/model.hpp"
#include "editor/ui/menus/debug_menu.hpp"
#include "editor/ui/menus/edit_menu.hpp"
#include "editor/ui/menus/file_menu.hpp"
#include "editor/ui/menus/help_menu.hpp"
#include "editor/ui/menus/map_menu.hpp"
#include "editor/ui/menus/tileset_menu.hpp"
#include "editor/ui/menus/view_menu.hpp"

namespace tactile::ui {

void update_menu_bar(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMainMenuBar()) {
    update_file_menu(model, dispatcher);
    update_edit_menu(model, dispatcher);
    update_view_menu(model, dispatcher);
    update_map_menu(model, dispatcher);
    update_tileset_menu(model, dispatcher);
    update_help_menu(model, dispatcher);
    update_debug_menu();

    ImGui::EndMainMenuBar();
  }
}

}  // namespace tactile::ui
