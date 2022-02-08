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

void Toolbar::Update(const Model& model, entt::dispatcher& dispatcher)
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
    dispatcher.enqueue<show_new_map_dialog_event>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (button(TAC_ICON_OPEN, "Open map", bw, bh)) {
    dispatcher.enqueue<show_open_map_dialog_event>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (button(TAC_ICON_SAVE, "Save", !model.IsClean(), bw, bh)) {
    dispatcher.enqueue<save_event>();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (button(TAC_ICON_UNDO, "Undo", model.CanUndo(), bw, bh)) {
    dispatcher.enqueue<undo_event>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (button(TAC_ICON_REDO, "Redo", model.CanRedo(), bw, bh)) {
    dispatcher.enqueue<redo_event>();
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
    dispatcher.enqueue<open_resize_map_dialog_event>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (button(TAC_ICON_TILESET, "Create tileset", true, bw, bh)) {
    dispatcher.enqueue<show_add_tileset_dialog_event>();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_STAMP,
                 "Stamp tool",
                 model.is_tool_active(tool_type::stamp),
                 model.is_tool_possible(tool_type::stamp),
                 bw,
                 bh)) {
    dispatcher.enqueue<select_tool_event>(tool_type::stamp);
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_BUCKET,
                 "Bucket tool",
                 model.is_tool_active(tool_type::bucket),
                 model.is_tool_possible(tool_type::bucket),
                 bw,
                 bh)) {
    dispatcher.enqueue<select_tool_event>(tool_type::bucket);
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_ERASER,
                 "Eraser tool",
                 model.is_tool_active(tool_type::eraser),
                 model.is_tool_possible(tool_type::eraser),
                 bw,
                 bh)) {
    dispatcher.enqueue<select_tool_event>(tool_type::eraser);
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_OBJECT_SELECTION,
                 "Object selection tool",
                 model.is_tool_active(tool_type::object_selection),
                 model.is_tool_possible(tool_type::object_selection),
                 bw,
                 bh)) {
    dispatcher.enqueue<select_tool_event>(tool_type::object_selection);
  }

  if (ToolButton(TAC_ICON_RECTANGLE,
                 "Rectangle tool",
                 model.is_tool_active(tool_type::rectangle),
                 model.is_tool_possible(tool_type::rectangle),
                 bw,
                 bh)) {
    dispatcher.enqueue<select_tool_event>(tool_type::object_selection);
  }

  if (ToolButton(TAC_ICON_ELLIPSE,
                 "Ellipse tool",
                 model.is_tool_active(tool_type::ellipse),
                 model.is_tool_possible(tool_type::ellipse),
                 bw,
                 bh)) {
    dispatcher.enqueue<select_tool_event>(tool_type::ellipse);
  }

  if (ToolButton(TAC_ICON_POINT,
                 "Point tool",
                 model.is_tool_active(tool_type::point),
                 model.is_tool_possible(tool_type::point),
                 bw,
                 bh)) {
    dispatcher.enqueue<select_tool_event>(tool_type::point);
  }

  end_docking_toolbar();
}

void Toolbar::SetVisible(const bool visible)
{
  mVisible = visible;
}

}  // namespace tactile
