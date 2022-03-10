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

#include "core/tool_type.hpp"
#include "core/utils/formatted_string.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/gui/dialogs/settings_dialog.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"

namespace tactile {

void EditMenu::update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (scoped::Menu menu{"Edit"}; menu.is_open()) {
    const auto canUndo = model.can_undo();
    const auto canRedo = model.can_redo();

    const FormattedString undoText{TAC_ICON_UNDO " Undo {}",
                                   canUndo ? model.get_undo_text() : ""};
    const FormattedString redoText{TAC_ICON_REDO " Redo {}",
                                   canRedo ? model.get_redo_text() : ""};

    if (ImGui::MenuItem(undoText.data(), TACTILE_PRIMARY_MOD "+Z", false, canUndo)) {
      dispatcher.enqueue<UndoEvent>();
    }

    if (ImGui::MenuItem(redoText.data(), TACTILE_PRIMARY_MOD "+Y", false, canRedo)) {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_STAMP " Stamp Tool",
                        "S",
                        model.is_tool_active(ToolType::stamp),
                        model.is_tool_possible(ToolType::stamp))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::stamp);
    }

    if (ImGui::MenuItem(TAC_ICON_BUCKET " Bucket Tool",
                        "B",
                        model.is_tool_active(ToolType::bucket),
                        model.is_tool_possible(ToolType::bucket))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::bucket);
    }

    if (ImGui::MenuItem(TAC_ICON_ERASER " Eraser Tool",
                        "E",
                        model.is_tool_active(ToolType::eraser),
                        model.is_tool_possible(ToolType::eraser))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::eraser);
    }

    if (ImGui::MenuItem(TAC_ICON_OBJECT_SELECTION " Object Selection Tool",
                        "Q",
                        model.is_tool_active(ToolType::object_selection),
                        model.is_tool_possible(ToolType::object_selection))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::object_selection);
    }

    if (ImGui::MenuItem(TAC_ICON_RECTANGLE " Rectangle Tool",
                        "R",
                        model.is_tool_active(ToolType::rectangle),
                        model.is_tool_possible(ToolType::rectangle))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::rectangle);
    }

    if (ImGui::MenuItem(TAC_ICON_ELLIPSE " Ellipse Tool",
                        "T",
                        model.is_tool_active(ToolType::ellipse),
                        model.is_tool_possible(ToolType::ellipse))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::ellipse);
    }

    if (ImGui::MenuItem(TAC_ICON_POINT " Point Tool",
                        "Y",
                        model.is_tool_active(ToolType::point),
                        model.is_tool_possible(ToolType::point))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::point);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_COMPONENT " Component Editor...",
                        nullptr,
                        false,
                        model.has_active_document())) {
      mComponentEditor.show(model);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_SETTINGS " Settings...", TACTILE_PRIMARY_MOD "+,")) {
      mSettingsDialog.show();
    }
  }
}

void EditMenu::update_windows(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  mSettingsDialog.update(model, dispatcher);
  mComponentEditor.update(model, dispatcher);
}

void EditMenu::show_settings_dialog()
{
  mSettingsDialog.show();
}

void EditMenu::show_component_editor(const DocumentModel& model)
{
  mComponentEditor.show(model);
}

}  // namespace tactile
