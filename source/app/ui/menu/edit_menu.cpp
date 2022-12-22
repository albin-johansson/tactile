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

#include "edit_menu.hpp"

#include <imgui.h>

#include "editor/shortcut/mappings.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/widget/menu_item.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {

void update_edit_menu()
{
  const auto& lang = get_current_language();

  if (Menu menu {lang.menu.edit.c_str()}; menu.is_open()) {
    menu_item(MenuAction::Undo, TACTILE_PRIMARY_MOD "+Z");
    menu_item(MenuAction::Redo, TACTILE_PRIMARY_MOD "+Y");

    ImGui::Separator();

    menu_item(MenuAction::StampTool, "S");
    menu_item(MenuAction::BucketTool, "B");
    menu_item(MenuAction::EraserTool, "E");
    menu_item(MenuAction::ObjectSelectionTool, "Q");
    menu_item(MenuAction::RectangleTool, "R");
    menu_item(MenuAction::EllipseTool, "T");
    menu_item(MenuAction::PointTool, "Y");

    ImGui::Separator();

    menu_item(MenuAction::ComponentEditor, TACTILE_PRIMARY_MOD "+Shift+C");

    ImGui::Separator();

    menu_item(MenuAction::OpenSettings, TACTILE_PRIMARY_MOD "+,");
  }
}

}  // namespace tactile::ui
