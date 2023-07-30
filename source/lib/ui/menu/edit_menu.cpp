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

#include "model/events/menu_events.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile {

void push_edit_menu(ModelView& model)
{
  const auto& strings = model.get_language_strings();

  if (const ui::Menu menu {strings.menu.edit.c_str()}; menu.is_open()) {
    if (ImGui::MenuItem(strings.action.undo.c_str(),
                        TACTILE_PRIMARY_MOD "+Z",
                        nullptr,
                        model.is_available(MenuAction::Undo))) {
      model.enqueue<MenuActionEvent>(MenuAction::Undo);
    }

    if (ImGui::MenuItem(strings.action.redo.c_str(),
                        TACTILE_PRIMARY_MOD "+Y",
                        nullptr,
                        model.is_available(MenuAction::Redo))) {
      model.enqueue<MenuActionEvent>(MenuAction::Redo);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.stamp_tool.c_str(),
                        "S",
                        model.is_selected(MenuAction::EnableStamp),
                        model.is_available(MenuAction::EnableStamp))) {
      model.enqueue<MenuActionEvent>(MenuAction::EnableStamp);
    }

    if (ImGui::MenuItem(strings.action.bucket_tool.c_str(),
                        "B",
                        model.is_selected(MenuAction::EnableBucket),
                        model.is_available(MenuAction::EnableBucket))) {
      model.enqueue<MenuActionEvent>(MenuAction::EnableBucket);
    }

    if (ImGui::MenuItem(strings.action.eraser_tool.c_str(),
                        "E",
                        model.is_selected(MenuAction::EnableEraser),
                        model.is_available(MenuAction::EnableEraser))) {
      model.enqueue<MenuActionEvent>(MenuAction::EnableEraser);
    }

    if (ImGui::MenuItem(strings.action.object_selection_tool.c_str(),
                        "Q",
                        model.is_selected(MenuAction::EnableObjectSelector),
                        model.is_available(MenuAction::EnableObjectSelector))) {
      model.enqueue<MenuActionEvent>(MenuAction::EnableObjectSelector);
    }

    if (ImGui::MenuItem(strings.action.rectangle_tool.c_str(),
                        "R",
                        model.is_selected(MenuAction::EnableRectangle),
                        model.is_available(MenuAction::EnableRectangle))) {
      model.enqueue<MenuActionEvent>(MenuAction::EnableRectangle);
    }

    if (ImGui::MenuItem(strings.action.ellipse_tool.c_str(),
                        "T",
                        model.is_selected(MenuAction::EnableEllipse),
                        model.is_available(MenuAction::EnableEllipse))) {
      model.enqueue<MenuActionEvent>(MenuAction::EnableEllipse);
    }

    if (ImGui::MenuItem(strings.action.point_tool.c_str(),
                        "Y",
                        model.is_selected(MenuAction::EnablePoint),
                        model.is_available(MenuAction::EnablePoint))) {
      model.enqueue<MenuActionEvent>(MenuAction::EnablePoint);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.component_editor.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+C",
                        nullptr,
                        model.is_available(MenuAction::OpenComponentEditor))) {
      model.enqueue<MenuActionEvent>(MenuAction::OpenComponentEditor);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.show_settings.c_str(),
                        TACTILE_PRIMARY_MOD "+,",
                        nullptr,
                        model.is_available(MenuAction::OpenSettings))) {
      model.enqueue<MenuActionEvent>(MenuAction::OpenSettings);
    }
  }
}

}  // namespace tactile
