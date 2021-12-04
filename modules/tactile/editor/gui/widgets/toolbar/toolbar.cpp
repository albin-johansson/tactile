#include "toolbar.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/tool_type.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/save_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/widgets/common/button.hpp"
#include "editor/gui/widgets/common/docking_toolbar.hpp"
#include "editor/gui/widgets/menus/file_menu.hpp"
#include "editor/gui/widgets/menus/map_menu.hpp"
#include "editor/model.hpp"
#include "tool_button.hpp"

namespace Tactile {

void Toolbar::Update(const Model& model, entt::dispatcher& dispatcher)
{
  if (!mVisible) { return; }

  static int axis = ImGuiAxis_X;
  constexpr auto bw = 24;
  constexpr auto bh = 24;

  BeginDockingToolbar("Toolbar", axis);
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

  if (Button(TAC_ICON_FILE, "Create new map", bw, bh)) {
    ShowAddMapDialog();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_OPEN, "Open map", bw, bh)) {
    ShowOpenMapDialog();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_SAVE, "Save", !model.IsClean(), bw, bh)) {
    dispatcher.enqueue<SaveEvent>();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_UNDO, "Undo", model.CanUndo(), bw, bh)) {
    dispatcher.enqueue<UndoEvent>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_REDO, "Redo", model.CanRedo(), bw, bh)) {
    dispatcher.enqueue<RedoEvent>();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_CENTER, "Center viewport", true, bw, bh)) {
    dispatcher.enqueue<CenterViewportEvent>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_RESIZE, "Resize map", true, bw, bh)) {
    dispatcher.enqueue<OpenResizeMapDialogEvent>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_TILESET, "Create tileset", true, bw, bh)) {
    ShowTilesetDialog();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_STAMP,
                 "Stamp tool",
                 model.IsStampActive(),
                 model.IsStampPossible(),
                 bw,
                 bh))
  {
    dispatcher.enqueue<SelectToolEvent>(ToolType::Stamp);
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_BUCKET,
                 "Bucket tool",
                 model.IsBucketActive(),
                 model.IsBucketPossible(),
                 bw,
                 bh))
  {
    dispatcher.enqueue<SelectToolEvent>(ToolType::Bucket);
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_ERASER,
                 "Eraser tool",
                 model.IsEraserActive(),
                 model.IsEraserPossible(),
                 bw,
                 bh))
  {
    dispatcher.enqueue<SelectToolEvent>(ToolType::Eraser);
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_OBJECT_SELECTION,
                 "Object selection tool",
                 model.IsObjectSelectionActive(),
                 model.IsObjectSelectionPossible(),
                 bw,
                 bh)) {
    dispatcher.enqueue<SelectToolEvent>(ToolType::ObjectSelection);
  }

  EndDockingToolbar();
}

void Toolbar::SetVisible(const bool visible)
{
  mVisible = visible;
}

}  // namespace Tactile
