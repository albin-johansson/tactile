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

#include "menu_item.hpp"

#include <imgui.h>

#include "editor/handler/menu_event_handler.hpp"

namespace tactile::ui {

void menu_item(const MenuAction action, const char* shortcut)
{
  const auto& item = get_menu_item(action);
  if (ImGui::MenuItem(item.text.c_str(), shortcut, item.selected, item.enabled)) {
    dispatch_menu_action(action);
  }
}

}  // namespace tactile::ui
