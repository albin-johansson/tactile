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

#include <concepts>  // invocable

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "model/event/command_events.hpp"
#include "model/event/tileset_events.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "model/tool/tool_manager.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/buttons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/windows.hpp"

namespace tactile::ui {
namespace {

constexpr auto toolbar_window_flags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
    ImGuiWindowFlags_NoMove;

constexpr uint32 toolbar_highlight_color = IM_COL32(0, 180, 0, 255);
constinit bool toolbar_visible = false;
constinit bool toolbar_hovered = false;
constinit bool toolbar_focused = false;
constinit float toolbar_width = 0;

void prepare_window_position(const ImVec2& offset = {})
{
  const auto window_pos = ImGui::GetWindowPos();
  const ImVec2 pos {window_pos.x + offset.x + 6.0f,
                    window_pos.y + offset.y + 6.0f + ImGui::GetFrameHeightWithSpacing()};
  const ImVec2 pivot {0.0f, 0.0f};

  ImGui::SetNextWindowPos(pos, ImGuiCond_Always, pivot);
  ImGui::SetNextWindowViewport(ImGui::GetWindowViewport()->ID);
}

void tool_button(const DocumentModel& model,
                 const ToolManager& tools,
                 entt::dispatcher& dispatcher,
                 const char* icon,
                 const char* tooltip,
                 const ToolType tool)
{
  const auto selected = tools.is_enabled(tool);

  if (selected) {
    ImGui::PushStyleColor(ImGuiCol_Button, toolbar_highlight_color);
  }

  if (icon_button(icon, tooltip, tools.is_available(model, tool))) {
    dispatcher.enqueue<SelectToolEvent>(tool);
  }

  if (selected) {
    ImGui::PopStyleColor();
  }
}

void show_extra_toolbar(std::invocable auto callable)
{
  const auto& style = ImGui::GetStyle();
  prepare_window_position({toolbar_width + style.ItemInnerSpacing.x, 0});

  if (Window extra {"##ToolbarWindowExtra", toolbar_window_flags}; extra.is_open()) {
    // Prevent other mouse events in the viewport by treating both toolbars as one
    if (!toolbar_hovered) {
      toolbar_hovered = ImGui::IsWindowHovered();
    }
    callable();
  }
}

}  // namespace

void update_map_viewport_toolbar(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  remove_tab_bar_from_next_window();

  prepare_window_position();
  ImGui::SetNextWindowBgAlpha(0.75f);

  StyleVar padding {ImGuiStyleVar_WindowPadding, {6, 6}};

  const auto& lang = get_current_language();
  const auto& document = model.view_map(model.active_document_id().value());
  const auto& tools = document.get_tools();

  if (Window window {"##ToolbarWindow", toolbar_window_flags}; window.is_open()) {
    toolbar_visible = true;
    toolbar_hovered = ImGui::IsWindowHovered();
    toolbar_focused = window.has_focus();
    toolbar_width = ImGui::GetWindowSize().x;

    const auto& commands = document.get_history();

    if (icon_button(TAC_ICON_UNDO, lang.misc.undo.c_str(), commands.can_undo())) {
      dispatcher.enqueue<UndoEvent>();
    }

    if (icon_button(TAC_ICON_REDO, lang.misc.redo.c_str(), commands.can_redo())) {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    if (icon_button(TAC_ICON_TILESET, lang.tooltip.create_tileset.c_str())) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_STAMP,
                lang.misc.stamp_tool.c_str(),
                ToolType::Stamp);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_ERASER,
                lang.misc.eraser_tool.c_str(),
                ToolType::Eraser);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_BUCKET,
                lang.misc.bucket_tool.c_str(),
                ToolType::Bucket);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_OBJECT_SELECTION,
                lang.misc.object_selection_tool.c_str(),
                ToolType::ObjectSelection);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_RECTANGLE,
                lang.misc.rectangle_tool.c_str(),
                ToolType::Rectangle);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_ELLIPSE,
                lang.misc.ellipse_tool.c_str(),
                ToolType::Ellipse);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_POINT,
                lang.misc.point_tool.c_str(),
                ToolType::Point);
  }
  else {
    toolbar_visible = false;
    toolbar_hovered = false;
    toolbar_focused = false;
  }

  if (toolbar_visible && tools.is_enabled(ToolType::Stamp)) {
    show_extra_toolbar([&] {
      const auto& tools = document.get_tools();
      const auto selected = tools.is_stamp_random();

      if (selected) {
        ImGui::PushStyleColor(ImGuiCol_Button, toolbar_highlight_color);
      }

      const auto& map = document.get_map();
      if (icon_button(TAC_ICON_STAMP_RANDOMIZER,
                      lang.tooltip.stamp_random_tile.c_str(),
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
  return toolbar_visible;
}

auto is_map_toolbar_hovered() -> bool
{
  return toolbar_hovered;
}

auto is_map_toolbar_focused() -> bool
{
  return toolbar_focused;
}

}  // namespace tactile::ui