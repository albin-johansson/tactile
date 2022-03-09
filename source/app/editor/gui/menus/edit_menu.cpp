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

void EditMenu::Update(const document_model& model, entt::dispatcher& dispatcher)
{
  if (scoped::menu menu{"Edit"}; menu.is_open()) {
    const auto canUndo = model.can_undo();
    const auto canRedo = model.can_redo();

    const formatted_string undoText{TAC_ICON_UNDO " Undo {}",
                                    canUndo ? model.get_undo_text() : ""};
    const formatted_string redoText{TAC_ICON_REDO " Redo {}",
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
                        model.is_tool_active(tool_type::stamp),
                        model.is_tool_possible(tool_type::stamp))) {
      dispatcher.enqueue<SelectToolEvent>(tool_type::stamp);
    }

    if (ImGui::MenuItem(TAC_ICON_BUCKET " Bucket Tool",
                        "B",
                        model.is_tool_active(tool_type::bucket),
                        model.is_tool_possible(tool_type::bucket))) {
      dispatcher.enqueue<SelectToolEvent>(tool_type::bucket);
    }

    if (ImGui::MenuItem(TAC_ICON_ERASER " Eraser Tool",
                        "E",
                        model.is_tool_active(tool_type::eraser),
                        model.is_tool_possible(tool_type::eraser))) {
      dispatcher.enqueue<SelectToolEvent>(tool_type::eraser);
    }

    if (ImGui::MenuItem(TAC_ICON_OBJECT_SELECTION " Object Selection Tool",
                        "Q",
                        model.is_tool_active(tool_type::object_selection),
                        model.is_tool_possible(tool_type::object_selection))) {
      dispatcher.enqueue<SelectToolEvent>(tool_type::object_selection);
    }

    if (ImGui::MenuItem(TAC_ICON_RECTANGLE " Rectangle Tool",
                        "R",
                        model.is_tool_active(tool_type::rectangle),
                        model.is_tool_possible(tool_type::rectangle))) {
      dispatcher.enqueue<SelectToolEvent>(tool_type::rectangle);
    }

    if (ImGui::MenuItem(TAC_ICON_ELLIPSE " Ellipse Tool",
                        "T",
                        model.is_tool_active(tool_type::ellipse),
                        model.is_tool_possible(tool_type::ellipse))) {
      dispatcher.enqueue<SelectToolEvent>(tool_type::ellipse);
    }

    if (ImGui::MenuItem(TAC_ICON_POINT " Point Tool",
                        "Y",
                        model.is_tool_active(tool_type::point),
                        model.is_tool_possible(tool_type::point))) {
      dispatcher.enqueue<SelectToolEvent>(tool_type::point);
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

void EditMenu::UpdateWindows(const document_model& model, entt::dispatcher& dispatcher)
{
  mSettingsDialog.update(model, dispatcher);
  mComponentEditor.update(model, dispatcher);
}

void EditMenu::OpenSettingsModal()
{
  mSettingsDialog.show();
}

void EditMenu::ShowComponentEditor(const document_model& model)
{
  mComponentEditor.show(model);
}

}  // namespace tactile
