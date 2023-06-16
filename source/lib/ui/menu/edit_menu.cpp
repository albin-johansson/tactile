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

#include "edit_menu.hpp"

#include <imgui.h>

#include "model/systems/language_system.hpp"
#include "model/systems/menu_system.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {

void push_edit_menu(const Model& model, Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  if (const Menu menu {strings.menu.edit.c_str()}; menu.is_open()) {
    push_menu_item(model, MenuAction::Undo, dispatcher);
    push_menu_item(model, MenuAction::Redo, dispatcher);

    ImGui::Separator();

    push_menu_item(model, MenuAction::EnableStamp, dispatcher);
    push_menu_item(model, MenuAction::EnableBucket, dispatcher);
    push_menu_item(model, MenuAction::EnableEraser, dispatcher);
    push_menu_item(model, MenuAction::EnableObjectSelector, dispatcher);
    push_menu_item(model, MenuAction::EnableRectangle, dispatcher);
    push_menu_item(model, MenuAction::EnableEllipse, dispatcher);
    push_menu_item(model, MenuAction::EnablePoint, dispatcher);

    ImGui::Separator();

    push_menu_item(model, MenuAction::OpenComponentEditor, dispatcher);

    ImGui::Separator();

    push_menu_item(model, MenuAction::OpenSettings, dispatcher);
  }
}

}  // namespace tactile::ui
