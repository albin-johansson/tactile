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

#include "menu_bar.hpp"

#include <imgui.h>

#include "ui/menu/edit_menu.hpp"
#include "ui/menu/file_menu.hpp"
#include "ui/menu/help_menu.hpp"
#include "ui/menu/map_menu.hpp"
#include "ui/menu/tileset_menu.hpp"
#include "ui/menu/view_menu.hpp"

namespace tactile {

void push_menu_bar(ModelView& model, MenuBarState& state)
{
  if (ImGui::BeginMainMenuBar()) {
    push_file_menu(model);
    push_edit_menu(model);
    push_view_menu(model);
    push_map_menu(model);
    push_tileset_menu(model);
    push_help_menu(model);
    push_debug_menu(model, state.debug_menu);

    ImGui::EndMainMenuBar();
  }
}

}  // namespace tactile
