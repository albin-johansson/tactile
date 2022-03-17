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

#include "editor/events/tileset_events.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"

namespace tactile {
namespace {

constexpr auto _window_flags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
    ImGuiWindowFlags_NoMove;

constexpr int _button_width = 24;
constexpr int _button_height = 24;

constinit bool _toolbar_visible = false;
constinit bool _toolbar_hovered = false;
constinit bool _toolbar_focused = false;

void _prepare_window_position()
{
  const auto windowPos = ImGui::GetWindowPos();
  const ImVec2 pos{windowPos.x + 6.0f,
                   windowPos.y + 6.0f + ImGui::GetFrameHeightWithSpacing()};
  const ImVec2 pivot{0.0f, 0.0f};

  ImGui::SetNextWindowPos(pos, ImGuiCond_Always, pivot);
  ImGui::SetNextWindowViewport(ImGui::GetWindowViewport()->ID);
}

void _tool_button(const DocumentModel& model,
                  entt::dispatcher& dispatcher,
                  const char* label,
                  const char* tooltip,
                  const ToolType tool)
{
  const auto selected = model.is_tool_active(tool);

  if (selected) {
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 180, 0, 0xFF));
  }

  if (button(label,
             tooltip,
             model.is_tool_possible(tool),
             _button_width,
             _button_height)) {
    dispatcher.enqueue<SelectToolEvent>(tool);
  }

  if (selected) {
    ImGui::PopStyleColor();
  }
}

}  // namespace

void show_viewport_toolbar(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  ImGuiWindowClass wc{};
  wc.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&wc);

  _prepare_window_position();
  ImGui::SetNextWindowBgAlpha(0.75f);

  scoped::StyleVar padding{ImGuiStyleVar_WindowPadding, {6, 6}};

  if (scoped::Window window{"##ViewportToolbarWindow", _window_flags}; window.is_open()) {
    _toolbar_visible = true;
    _toolbar_hovered = ImGui::IsWindowHovered();
    _toolbar_focused = window.has_focus();

    if (button(TAC_ICON_UNDO, "Undo", model.can_undo(), _button_width, _button_height)) {
      dispatcher.enqueue<UndoEvent>();
    }

    if (button(TAC_ICON_REDO, "Redo", model.can_redo(), _button_width, _button_height)) {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::Separator();

    if (button(TAC_ICON_TILESET, "Create tileset", true, _button_width, _button_height)) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    ImGui::Separator();

    _tool_button(model, dispatcher, TAC_ICON_STAMP, "Stamp tool", ToolType::stamp);
    _tool_button(model, dispatcher, TAC_ICON_ERASER, "Eraser tool", ToolType::eraser);
    _tool_button(model, dispatcher, TAC_ICON_BUCKET, "Bucket tool", ToolType::bucket);

    ImGui::Separator();

    _tool_button(model,
                 dispatcher,
                 TAC_ICON_OBJECT_SELECTION,
                 "Object selection tool",
                 ToolType::object_selection);

    _tool_button(model,
                 dispatcher,
                 TAC_ICON_RECTANGLE,
                 "Rectangle tool",
                 ToolType::rectangle);

    _tool_button(model, dispatcher, TAC_ICON_ELLIPSE, "Ellipse tool", ToolType::ellipse);
    _tool_button(model, dispatcher, TAC_ICON_POINT, "Point tool", ToolType::point);
  }
  else {
    _toolbar_visible = false;
    _toolbar_hovered = false;
    _toolbar_focused = false;
  }
}

auto is_toolbar_visible() -> bool
{
  return _toolbar_visible;
}

auto is_toolbar_hovered() -> bool
{
  return _toolbar_hovered;
}

auto is_toolbar_focused() -> bool
{
  return _toolbar_focused;
}

}  // namespace tactile