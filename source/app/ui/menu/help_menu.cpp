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

#include "help_menu.hpp"

#include <imgui.h>

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {

void update_help_menu()
{
  const auto& lang = get_current_language();

  if (Menu menu {lang.menu.help.c_str()}; menu.is_open()) {
    ui_menu_item(MenuAction::AboutTactile);
    ui_menu_item(MenuAction::AboutDearImGui);

    ImGui::Separator();
    ui_menu_item(MenuAction::ReportIssue);

    ImGui::Separator();
    ui_menu_item(MenuAction::Credits);
  }
}

}  // namespace tactile::ui
