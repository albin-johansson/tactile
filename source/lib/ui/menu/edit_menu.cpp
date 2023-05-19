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

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/systems/menu_system.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {

void show_edit_menu(const Model& model, Dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (const Menu menu {lang.menu.edit.c_str()}; menu.is_open()) {
    show_menu_item(model, MenuAction::Undo, dispatcher);
    show_menu_item(model, MenuAction::Redo, dispatcher);

    ImGui::Separator();

    show_menu_item(model, MenuAction::EnableStamp, dispatcher);
    show_menu_item(model, MenuAction::EnableBucket, dispatcher);
    show_menu_item(model, MenuAction::EnableEraser, dispatcher);
    show_menu_item(model, MenuAction::EnableObjectSelector, dispatcher);
    show_menu_item(model, MenuAction::EnableRectangle, dispatcher);
    show_menu_item(model, MenuAction::EnableEllipse, dispatcher);
    show_menu_item(model, MenuAction::EnablePoint, dispatcher);

    ImGui::Separator();

    show_menu_item(model, MenuAction::OpenComponentEditor, dispatcher);

    ImGui::Separator();

    show_menu_item(model, MenuAction::OpenSettings, dispatcher);
  }
}

}  // namespace tactile::ui
