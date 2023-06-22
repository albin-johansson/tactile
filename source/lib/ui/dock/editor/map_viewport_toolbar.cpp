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

#include "map_viewport_toolbar.hpp"

#include <concepts>  // invocable

#include <imgui.h>
#include <imgui_internal.h>

#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/event/command_events.hpp"
#include "model/event/tileset_events.hpp"
#include "model/event/tool_events.hpp"
#include "model/systems/language_system.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"
#include "ui/widget/windows.hpp"

namespace tactile::ui {
namespace {

inline constexpr auto kToolbarWindowFlags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
    ImGuiWindowFlags_NoMove;

// inline constexpr uint32 kToolbarHighlightColor = IM_COL32(0, 180, 0, 255);

struct MapViewportToolbarState final {
  float width {};
  bool visible {};
  bool has_hover {};
  bool has_focus {};
};

inline constinit MapViewportToolbarState gToolbarState;

void _prepare_window_position(const ImVec2& offset = {})
{
  const auto window_pos = ImGui::GetWindowPos();
  const ImVec2 pos {window_pos.x + offset.x + 6.0f,
                    window_pos.y + offset.y + 6.0f + ImGui::GetFrameHeightWithSpacing()};
  const ImVec2 pivot {0.0f, 0.0f};

  ImGui::SetNextWindowPos(pos, ImGuiCond_Always, pivot);
  ImGui::SetNextWindowViewport(ImGui::GetWindowViewport()->ID);
}

void _tool_button([[maybe_unused]] const Model& model,
                  [[maybe_unused]] const ToolType tool,
                  [[maybe_unused]] const char* icon,
                  [[maybe_unused]] const char* tooltip,
                  [[maybe_unused]] Dispatcher& dispatcher)
{
  //  const auto selected = tools.is_enabled(tool);
  //
  //  if (selected) {
  //    ImGui::PushStyleColor(ImGuiCol_Button, kToolbarHighlightColor);
  //  }
  //
  //  if (ui_icon_button(icon, tooltip, tools.is_available(model, tool))) {
  //    dispatcher.enqueue<SelectToolEvent>(tool);
  //  }
  //
  //  if (selected) {
  //    ImGui::PopStyleColor();
  //  }
}

void _push_extra_toolbar(std::invocable auto callable)
{
  const auto& style = ImGui::GetStyle();
  _prepare_window_position({gToolbarState.width + style.ItemInnerSpacing.x, 0});

  if (const Window extra {"##ToolbarWindowExtra", kToolbarWindowFlags}; extra.is_open()) {
    // Prevent other mouse events in the viewport by treating both toolbars as one
    if (!gToolbarState.has_hover) {
      gToolbarState.has_hover = ImGui::IsWindowHovered();
    }
    callable();
  }
}

}  // namespace

void show_map_viewport_toolbar(const Model& model, Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  remove_tab_bar_from_next_window();

  _prepare_window_position();
  ImGui::SetNextWindowBgAlpha(0.75f);

  const StyleVar padding {ImGuiStyleVar_WindowPadding, {6, 6}};

  const auto document_entity = sys::get_active_document(model);
  // const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& command_stack = model.get<CommandStack>(document_entity);
  // const auto& tools = map_document.get_tools();

  if (const Window window {"##ToolbarWindow", kToolbarWindowFlags}; window.is_open()) {
    gToolbarState.visible = true;
    gToolbarState.has_hover = ImGui::IsWindowHovered();
    gToolbarState.has_focus = window.has_focus();
    gToolbarState.width = ImGui::GetWindowSize().x;

    if (push_icon_button(TAC_ICON_UNDO,
                         strings.misc.undo.c_str(),
                         command_stack.can_undo())) {
      dispatcher.enqueue<UndoEvent>();
    }

    if (push_icon_button(TAC_ICON_REDO,
                         strings.misc.redo.c_str(),
                         command_stack.can_redo())) {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    if (push_icon_button(TAC_ICON_TILESET, strings.tooltip.create_tileset.c_str())) {
      dispatcher.enqueue<ShowNewTilesetDialogEvent>();
    }

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    _tool_button(model,
                 ToolType::Stamp,
                 TAC_ICON_STAMP,
                 strings.misc.stamp_tool.c_str(),
                 dispatcher);

    _tool_button(model,
                 ToolType::Eraser,
                 TAC_ICON_ERASER,
                 strings.misc.eraser_tool.c_str(),
                 dispatcher);

    _tool_button(model,
                 ToolType::Bucket,
                 TAC_ICON_BUCKET,
                 strings.misc.bucket_tool.c_str(),
                 dispatcher);

    _tool_button(model,
                 ToolType::ObjectSelection,
                 TAC_ICON_OBJECT_SELECTION,
                 strings.misc.object_selection_tool.c_str(),
                 dispatcher);

    _tool_button(model,
                 ToolType::Rectangle,
                 TAC_ICON_RECTANGLE,
                 strings.misc.rectangle_tool.c_str(),
                 dispatcher);

    _tool_button(model,
                 ToolType::Ellipse,
                 TAC_ICON_ELLIPSE,
                 strings.misc.ellipse_tool.c_str(),
                 dispatcher);

    _tool_button(model,
                 ToolType::Point,
                 TAC_ICON_POINT,
                 strings.misc.point_tool.c_str(),
                 dispatcher);
  }
  else {
    gToolbarState.visible = false;
    gToolbarState.has_hover = false;
    gToolbarState.has_focus = false;
  }

  //  if (gToolbarState.visible && tools.is_enabled(ToolType::Stamp)) {
  //    show_extra_toolbar([&] {
  //      const auto& tools = map_document.get_tools();
  //      const auto selected = tools.is_stamp_random();
  //
  //      if (selected) {
  //        ImGui::PushStyleColor(ImGuiCol_Button, kToolbarHighlightColor);
  //      }
  //
  //      const auto& map = map_document.get_map();
  //      if (ui_icon_button(TAC_ICON_STAMP_RANDOMIZER,
  //                         strings.tooltip.stamp_random_tile.c_str(),
  //                         map.is_stamp_randomizer_possible())) {
  //        dispatcher.enqueue<SetStampRandomizerEvent>(!selected);
  //      }
  //
  //      if (selected) {
  //        ImGui::PopStyleColor();
  //      }
  //    });
  //  }
}

auto is_map_toolbar_visible() -> bool
{
  return gToolbarState.visible;
}

auto is_map_toolbar_hovered() -> bool
{
  return gToolbarState.has_hover;
}

auto is_map_toolbar_focused() -> bool
{
  return gToolbarState.has_focus;
}

}  // namespace tactile::ui