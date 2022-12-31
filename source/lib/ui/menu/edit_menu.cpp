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

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {

void update_edit_menu(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (const Menu menu {lang.menu.edit.c_str()}; menu.is_open()) {
    ui_menu_item(dispatcher, MenuAction::Undo, TACTILE_PRIMARY_MOD "+Z");
    ui_menu_item(dispatcher, MenuAction::Redo, TACTILE_PRIMARY_MOD "+Y");

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::StampTool, "S");
    ui_menu_item(dispatcher, MenuAction::BucketTool, "B");
    ui_menu_item(dispatcher, MenuAction::EraserTool, "E");
    ui_menu_item(dispatcher, MenuAction::ObjectSelectionTool, "Q");
    ui_menu_item(dispatcher, MenuAction::RectangleTool, "R");
    ui_menu_item(dispatcher, MenuAction::EllipseTool, "T");
    ui_menu_item(dispatcher, MenuAction::PointTool, "Y");

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::ComponentEditor, TACTILE_PRIMARY_MOD "+Shift+C");

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::OpenSettings, TACTILE_PRIMARY_MOD "+,");
  }
}

}  // namespace tactile::ui
