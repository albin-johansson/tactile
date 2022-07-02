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

#include "map_viewport_toolbar.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/documents/map_document.hpp"
#include "core/events/command_events.hpp"
#include "core/events/tileset_events.hpp"
#include "core/events/tool_events.hpp"
#include "core/model.hpp"
#include "core/tools/tool_manager.hpp"
#include "editor/ui/common/button.hpp"
#include "editor/ui/common/style.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"

namespace tactile::ui {
namespace {

constexpr auto _window_flags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
    ImGuiWindowFlags_NoMove;

constinit bool  _toolbar_visible = false;
constinit bool  _toolbar_hovered = false;
constinit bool  _toolbar_focused = false;
constinit float _toolbar_width = 0;

constexpr uint32 _highlight_color = IM_COL32(0, 180, 0, 255);

void _prepare_window_position(const ImVec2& offset = {})
{
  const auto   windowPos = ImGui::GetWindowPos();
  const ImVec2 pos{windowPos.x + offset.x + 6.0f,
                   windowPos.y + offset.y + 6.0f + ImGui::GetFrameHeightWithSpacing()};
  const ImVec2 pivot{0.0f, 0.0f};

  ImGui::SetNextWindowPos(pos, ImGuiCond_Always, pivot);
  ImGui::SetNextWindowViewport(ImGui::GetWindowViewport()->ID);
}

void _tool_button(const DocumentModel& model,
                  const ToolManager&   tools,
                  entt::dispatcher&    dispatcher,
                  const char*          icon,
                  const char*          tooltip,
                  const ToolType       tool)
{
  const auto selected = tools.is_enabled(tool);

  if (selected) {
    ImGui::PushStyleColor(ImGuiCol_Button, _highlight_color);
  }

  if (icon_button(icon, tooltip, tools.is_available(model, tool))) {
    dispatcher.enqueue<SelectToolEvent>(tool);
  }

  if (selected) {
    ImGui::PopStyleColor();
  }
}

void _show_extra_toolbar(auto callable)
{
  const auto& style = ImGui::GetStyle();
  _prepare_window_position({_toolbar_width + style.ItemInnerSpacing.x, 0});

  if (Window extra{"##ToolbarWindowExtra", _window_flags}; extra.is_open()) {
    /* Prevent other mouse events in the viewport by treating both toolbars as one */
    if (!_toolbar_hovered) {
      _toolbar_hovered = ImGui::IsWindowHovered();
    }
    callable();
  }
}

}  // namespace

void update_map_viewport_toolbar(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  remove_tab_bar_from_next_window();

  _prepare_window_position();
  ImGui::SetNextWindowBgAlpha(0.75f);

  StyleVar padding{ImGuiStyleVar_WindowPadding, {6, 6}};

  const auto& document = model.view_map(model.active_document_id().value());
  const auto& tools = document.get_tools();

  if (Window window{"##ToolbarWindow", _window_flags}; window.is_open()) {
    _toolbar_visible = true;
    _toolbar_hovered = ImGui::IsWindowHovered();
    _toolbar_focused = window.has_focus();
    _toolbar_width = ImGui::GetWindowSize().x;

    const auto& commands = document.get_history();

    if (icon_button(TAC_ICON_UNDO, "Undo", commands.can_undo())) {
      dispatcher.enqueue<UndoEvent>();
    }

    if (icon_button(TAC_ICON_REDO, "Redo", commands.can_redo())) {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    if (icon_button(TAC_ICON_TILESET, "Create tileset")) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    _tool_button(model, tools, dispatcher, TAC_ICON_STAMP, "Stamp tool", ToolType::Stamp);
    _tool_button(model,
                 tools,
                 dispatcher,
                 TAC_ICON_ERASER,
                 "Eraser tool",
                 ToolType::Eraser);
    _tool_button(model,
                 tools,
                 dispatcher,
                 TAC_ICON_BUCKET,
                 "Bucket tool",
                 ToolType::Bucket);
    _tool_button(model,
                 tools,
                 dispatcher,
                 TAC_ICON_OBJECT_SELECTION,
                 "Object selection tool",
                 ToolType::ObjectSelection);
    _tool_button(model,
                 tools,
                 dispatcher,
                 TAC_ICON_RECTANGLE,
                 "Rectangle tool",
                 ToolType::Rectangle);
    _tool_button(model,
                 tools,
                 dispatcher,
                 TAC_ICON_ELLIPSE,
                 "Ellipse tool",
                 ToolType::Ellipse);
    _tool_button(model, tools, dispatcher, TAC_ICON_POINT, "Point tool", ToolType::Point);
  }
  else {
    _toolbar_visible = false;
    _toolbar_hovered = false;
    _toolbar_focused = false;
  }

  if (_toolbar_visible && tools.is_enabled(ToolType::Stamp)) {
    _show_extra_toolbar([&] {
      const auto& tools = document.get_tools();
      const auto  selected = tools.is_stamp_random();

      if (selected) {
        ImGui::PushStyleColor(ImGuiCol_Button, _highlight_color);
      }

      const auto& map = document.get_map();
      if (icon_button(TAC_ICON_STAMP_RANDOMIZER,
                      "Stamp random tile",
                      map.is_stamp_randomizer_possible())) {
        dispatcher.enqueue<SetStampRandomizerEvent>(!selected);
      }

      if (selected) {
        ImGui::PopStyleColor();
      }
    });
  }
}

auto is_map_toolbar_visible() -> bool
{
  return _toolbar_visible;
}

auto is_map_toolbar_hovered() -> bool
{
  return _toolbar_hovered;
}

auto is_map_toolbar_focused() -> bool
{
  return _toolbar_focused;
}

}  // namespace tactile::ui