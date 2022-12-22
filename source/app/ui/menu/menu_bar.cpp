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

#include "debug_menu.hpp"
#include "edit_menu.hpp"
#include "file_menu.hpp"
#include "help_menu.hpp"
#include "map_menu.hpp"
#include "model/model.hpp"
#include "tileset_menu.hpp"
#include "view_menu.hpp"

namespace tactile::ui {

void update_menu_bar(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMainMenuBar()) {
    update_file_menu(model, dispatcher);
    update_edit_menu();
    update_view_menu();
    update_map_menu();
    update_tileset_menu();
    update_help_menu();
    update_debug_menu();

    ImGui::EndMainMenuBar();
  }
}

}  // namespace tactile::ui
