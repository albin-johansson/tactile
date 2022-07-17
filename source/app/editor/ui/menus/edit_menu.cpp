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

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/document/map_document.hpp"
#include "core/event/command_events.hpp"
#include "core/event/tool_events.hpp"
#include "core/model.hpp"
#include "core/tool/tool_type.hpp"
#include "core/util/formatted_string.hpp"
#include "editor/lang/language.hpp"
#include "editor/lang/strings.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/shared/dialog_state.hpp"
#include "editor/ui/shared/dialogs.hpp"

namespace tactile::ui {

void update_edit_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (Menu menu {lang.menu.edit.c_str()}; menu.is_open()) {
    const auto* document = model.active_document();

    const auto canUndo = document && document->get_history().can_undo();
    const auto canRedo = document && document->get_history().can_redo();

    const std::string undoAction = canUndo ? document->get_history().get_undo_text() : "";
    const std::string redoAction = canRedo ? document->get_history().get_redo_text() : "";

    const FormattedString undoText {"{} {}", lang.action.undo, undoAction};
    const FormattedString redoText {"{} {}", lang.action.redo, redoAction};

    if (ImGui::MenuItem(undoText.data(), TACTILE_PRIMARY_MOD "+Z", false, canUndo)) {
      dispatcher.enqueue<UndoEvent>();
    }

    if (ImGui::MenuItem(redoText.data(), TACTILE_PRIMARY_MOD "+Y", false, canRedo)) {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::Separator();

    const auto* map = dynamic_cast<const MapDocument*>(document);

    const auto isToolActive = [&](const ToolType tool) {
      return map && map->get_tools().is_enabled(tool);
    };

    const auto isToolPossible = [&](const ToolType tool) {
      return map && map->get_tools().is_available(model, tool);
    };

    if (ImGui::MenuItem(lang.action.stamp_tool.c_str(),
                        "S",
                        isToolActive(ToolType::Stamp),
                        isToolPossible(ToolType::Stamp))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Stamp);
    }

    if (ImGui::MenuItem(lang.action.bucket_tool.c_str(),
                        "B",
                        isToolActive(ToolType::Bucket),
                        isToolPossible(ToolType::Bucket))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Bucket);
    }

    if (ImGui::MenuItem(lang.action.eraser_tool.c_str(),
                        "E",
                        isToolActive(ToolType::Eraser),
                        isToolPossible(ToolType::Eraser))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Eraser);
    }

    if (ImGui::MenuItem(lang.action.object_selection_tool.c_str(),
                        "Q",
                        isToolActive(ToolType::ObjectSelection),
                        isToolPossible(ToolType::ObjectSelection))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::ObjectSelection);
    }

    if (ImGui::MenuItem(lang.action.rectangle_tool.c_str(),
                        "R",
                        isToolActive(ToolType::Rectangle),
                        isToolPossible(ToolType::Rectangle))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Rectangle);
    }

    if (ImGui::MenuItem(lang.action.ellipse_tool.c_str(),
                        "T",
                        isToolActive(ToolType::Ellipse),
                        isToolPossible(ToolType::Ellipse))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Ellipse);
    }

    if (ImGui::MenuItem(lang.action.point_tool.c_str(),
                        "Y",
                        isToolActive(ToolType::Point),
                        isToolPossible(ToolType::Point))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Point);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.show_component_editor.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+C",
                        false,
                        map != nullptr)) {
      get_dialogs().component_editor.show(model);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.show_settings.c_str(), TACTILE_PRIMARY_MOD "+,")) {
      get_dialogs().settings.show();
    }
  }

  auto& dialogs = get_dialogs();
  dialogs.settings.update(model, dispatcher);
  dialogs.component_editor.update(model, dispatcher);
}

void show_settings_dialog()
{
  get_dialogs().settings.show();
}

void show_component_editor(const DocumentModel& model)
{
  get_dialogs().component_editor.show(model);
}

}  // namespace tactile::ui
