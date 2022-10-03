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
#include "core/util/fmt.hpp"
#include "editor/shortcut/mappings.hpp"
#include "editor/ui/dialog/dialog_state.hpp"
#include "editor/ui/dialog/dialogs.hpp"
#include "editor/ui/widget/scoped.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::ui {

void update_edit_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (Menu menu {lang.menu.edit.c_str()}; menu.is_open()) {
    const auto* document = model.active_document();
    const auto* map_doc = dynamic_cast<const MapDocument*>(document);

    const auto can_undo = document && document->get_history().can_undo();
    const auto can_redo = document && document->get_history().can_redo();

    const FmtString undo_text {"{} {}",
                               lang.action.undo,
                               can_undo ? document->get_history().get_undo_text() : ""};
    const FmtString redo_text {"{} {}",
                               lang.action.redo,
                               can_redo ? document->get_history().get_redo_text() : ""};

    if (ImGui::MenuItem(undo_text.data(), TACTILE_PRIMARY_MOD "+Z", false, can_undo)) {
      dispatcher.enqueue<UndoEvent>();
    }

    if (ImGui::MenuItem(redo_text.data(), TACTILE_PRIMARY_MOD "+Y", false, can_redo)) {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::Separator();

    const auto is_tool_active = [&](const ToolType tool) {
      return map_doc && map_doc->get_tools().is_enabled(tool);
    };

    const auto is_tool_possible = [&](const ToolType tool) {
      return map_doc && map_doc->get_tools().is_available(model, tool);
    };

    if (ImGui::MenuItem(lang.action.stamp_tool.c_str(),
                        "S",
                        is_tool_active(ToolType::Stamp),
                        is_tool_possible(ToolType::Stamp))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Stamp);
    }

    if (ImGui::MenuItem(lang.action.bucket_tool.c_str(),
                        "B",
                        is_tool_active(ToolType::Bucket),
                        is_tool_possible(ToolType::Bucket))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Bucket);
    }

    if (ImGui::MenuItem(lang.action.eraser_tool.c_str(),
                        "E",
                        is_tool_active(ToolType::Eraser),
                        is_tool_possible(ToolType::Eraser))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Eraser);
    }

    if (ImGui::MenuItem(lang.action.object_selection_tool.c_str(),
                        "Q",
                        is_tool_active(ToolType::ObjectSelection),
                        is_tool_possible(ToolType::ObjectSelection))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::ObjectSelection);
    }

    if (ImGui::MenuItem(lang.action.rectangle_tool.c_str(),
                        "R",
                        is_tool_active(ToolType::Rectangle),
                        is_tool_possible(ToolType::Rectangle))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Rectangle);
    }

    if (ImGui::MenuItem(lang.action.ellipse_tool.c_str(),
                        "T",
                        is_tool_active(ToolType::Ellipse),
                        is_tool_possible(ToolType::Ellipse))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Ellipse);
    }

    if (ImGui::MenuItem(lang.action.point_tool.c_str(),
                        "Y",
                        is_tool_active(ToolType::Point),
                        is_tool_possible(ToolType::Point))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Point);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.show_component_editor.c_str(),
                        TACTILE_PRIMARY_MOD "+Shift+C",
                        false,
                        document && document->is_map())) {
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
