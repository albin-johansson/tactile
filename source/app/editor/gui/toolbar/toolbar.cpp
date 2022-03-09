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

#include "toolbar.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/tool_type.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/misc_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/docking_toolbar.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/menus/map_menu.hpp"
#include "editor/model.hpp"
#include "tool_button.hpp"

namespace tactile {

void Toolbar::Update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (!mVisible) {
    return;
  }

  static int axis = ImGuiAxis_X;
  constexpr auto bw = 24;
  constexpr auto bh = 24;

  begin_docking_toolbar("Toolbar", axis);
  mHasFocus = ImGui::IsWindowFocused();

  auto separate = [] {
    if (axis == ImGuiAxis_X) {
      ImGui::SameLine();
    }
    ImGui::Spacing();

    if (axis == ImGuiAxis_X) {
      ImGui::SameLine();
    }
    ImGui::Spacing();
  };

  if (button(TAC_ICON_FILE, "Create new map", bw, bh)) {
    dispatcher.enqueue<ShowNewMapDialogEvent>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (button(TAC_ICON_OPEN, "Open map", bw, bh)) {
    dispatcher.enqueue<ShowOpenMapDialogEvent>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (button(TAC_ICON_SAVE, "Save", !model.is_clean(), bw, bh)) {
    dispatcher.enqueue<SaveEvent>();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (button(TAC_ICON_UNDO, "Undo", model.can_undo(), bw, bh)) {
    dispatcher.enqueue<UndoEvent>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (button(TAC_ICON_REDO, "Redo", model.can_redo(), bw, bh)) {
    dispatcher.enqueue<RedoEvent>();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (button(TAC_ICON_CENTER, "Center viewport", true, bw, bh)) {
    dispatcher.enqueue<CenterViewportEvent>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (button(TAC_ICON_RESIZE, "Resize map", true, bw, bh)) {
    dispatcher.enqueue<OpenResizeMapDialogEvent>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (button(TAC_ICON_TILESET, "Create tileset", true, bw, bh)) {
    dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_STAMP,
                 "Stamp tool",
                 model.is_tool_active(ToolType::stamp),
                 model.is_tool_possible(ToolType::stamp),
                 bw,
                 bh)) {
    dispatcher.enqueue<SelectToolEvent>(ToolType::stamp);
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_BUCKET,
                 "Bucket tool",
                 model.is_tool_active(ToolType::bucket),
                 model.is_tool_possible(ToolType::bucket),
                 bw,
                 bh)) {
    dispatcher.enqueue<SelectToolEvent>(ToolType::bucket);
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_ERASER,
                 "Eraser tool",
                 model.is_tool_active(ToolType::eraser),
                 model.is_tool_possible(ToolType::eraser),
                 bw,
                 bh)) {
    dispatcher.enqueue<SelectToolEvent>(ToolType::eraser);
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_OBJECT_SELECTION,
                 "Object selection tool",
                 model.is_tool_active(ToolType::object_selection),
                 model.is_tool_possible(ToolType::object_selection),
                 bw,
                 bh)) {
    dispatcher.enqueue<SelectToolEvent>(ToolType::object_selection);
  }

  if (ToolButton(TAC_ICON_RECTANGLE,
                 "Rectangle tool",
                 model.is_tool_active(ToolType::rectangle),
                 model.is_tool_possible(ToolType::rectangle),
                 bw,
                 bh)) {
    dispatcher.enqueue<SelectToolEvent>(ToolType::rectangle);
  }

  if (ToolButton(TAC_ICON_ELLIPSE,
                 "Ellipse tool",
                 model.is_tool_active(ToolType::ellipse),
                 model.is_tool_possible(ToolType::ellipse),
                 bw,
                 bh)) {
    dispatcher.enqueue<SelectToolEvent>(ToolType::ellipse);
  }

  if (ToolButton(TAC_ICON_POINT,
                 "Point tool",
                 model.is_tool_active(ToolType::point),
                 model.is_tool_possible(ToolType::point),
                 bw,
                 bh)) {
    dispatcher.enqueue<SelectToolEvent>(ToolType::point);
  }

  end_docking_toolbar();
}

void Toolbar::SetVisible(const bool visible)
{
  mVisible = visible;
}

}  // namespace tactile
