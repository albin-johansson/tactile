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

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/tools/tool_type.hpp"
#include "core/utils/formatted_string.hpp"
#include "editor/commands/command_stack.hpp"
#include "core/documents/map_document.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/gui/components/component_editor.hpp"
#include "editor/gui/dialogs/settings_dialog.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto _get_settings_dialog() -> SettingsDialog&
{
  static SettingsDialog dialog;
  return dialog;
}

[[nodiscard]] auto _get_component_editor() -> ComponentEditor&
{
  static ComponentEditor editor;
  return editor;
}

}  // namespace

void update_edit_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (scoped::Menu menu{"Edit"}; menu.is_open()) {
    const auto* document = model.active_document();

    const auto canUndo = document && document->get_history().can_undo();
    const auto canRedo = document && document->get_history().can_redo();

    const auto undoText =
        FormattedString{TAC_ICON_UNDO " Undo {}",
                        canUndo ? document->get_history().get_undo_text() : ""};
    const auto redoText =
        FormattedString{TAC_ICON_REDO " Redo {}",
                        canRedo ? document->get_history().get_redo_text() : ""};

    if (ImGui::MenuItem(undoText.data(), TACTILE_PRIMARY_MOD "+Z", false, canUndo)) {
      dispatcher.enqueue<UndoEvent>();
    }

    if (ImGui::MenuItem(redoText.data(), TACTILE_PRIMARY_MOD "+Y", false, canRedo)) {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::Separator();

    const auto* map = dynamic_cast<const MapDocument*>(document);

    if (ImGui::MenuItem(TAC_ICON_STAMP " Stamp Tool",
                        "S",
                        map && map->is_tool_active(ToolType::Stamp),
                        map && map->is_tool_possible(ToolType::Stamp))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Stamp);
    }

    if (ImGui::MenuItem(TAC_ICON_BUCKET " Bucket Tool",
                        "B",
                        map && map->is_tool_active(ToolType::Bucket),
                        map && map->is_tool_possible(ToolType::Bucket))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Bucket);
    }

    if (ImGui::MenuItem(TAC_ICON_ERASER " Eraser Tool",
                        "E",
                        map && map->is_tool_active(ToolType::Eraser),
                        map && map->is_tool_possible(ToolType::Eraser))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Eraser);
    }

    if (ImGui::MenuItem(TAC_ICON_OBJECT_SELECTION " Object Selection Tool",
                        "Q",
                        map && map->is_tool_active(ToolType::ObjectSelection),
                        map && map->is_tool_possible(ToolType::ObjectSelection))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::ObjectSelection);
    }

    if (ImGui::MenuItem(TAC_ICON_RECTANGLE " Rectangle Tool",
                        "R",
                        map && map->is_tool_active(ToolType::Rectangle),
                        map && map->is_tool_possible(ToolType::Rectangle))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Rectangle);
    }

    if (ImGui::MenuItem(TAC_ICON_ELLIPSE " Ellipse Tool",
                        "T",
                        map && map->is_tool_active(ToolType::Ellipse),
                        map && map->is_tool_possible(ToolType::Ellipse))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Ellipse);
    }

    if (ImGui::MenuItem(TAC_ICON_POINT " Point Tool",
                        "Y",
                        map && map->is_tool_active(ToolType::Point),
                        map && map->is_tool_possible(ToolType::Point))) {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Point);
    }

    ImGui::Separator();

    // TODO make sure shortcut rule matches this
    if (ImGui::MenuItem(TAC_ICON_COMPONENT " Component Editor...",
                        TACTILE_PRIMARY_MOD "+Shift+C",
                        false,
                        map != nullptr)) {
      _get_component_editor().show(model);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_SETTINGS " Settings...", TACTILE_PRIMARY_MOD "+,")) {
      _get_settings_dialog().show();
    }
  }

  _get_settings_dialog().update(model, dispatcher);
  _get_component_editor().update(model, dispatcher);
}

void show_settings_dialog()
{
  _get_settings_dialog().show();
}

void show_component_editor(const DocumentModel& model)
{
  _get_component_editor().show(model);
}

}  // namespace tactile
