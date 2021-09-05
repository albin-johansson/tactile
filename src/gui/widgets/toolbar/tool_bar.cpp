#include "tool_bar.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/model.hpp"
#include "core/mouse_tool_type.hpp"
#include "events/command_events.hpp"
#include "events/tool_events.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/common/button.hpp"
#include "gui/widgets/common/docking_toolbar.hpp"
#include "gui/widgets/menus/edit_menu.hpp"
#include "gui/widgets/menus/file_menu.hpp"
#include "gui/widgets/toolbar/tool_button.hpp"

namespace Tactile {

void UpdateToolbarWidget(const Model& model, entt::dispatcher& dispatcher)
{
  static int axis = ImGuiAxis_X;
  BeginDockingToolbar("Toolbar", axis);

  if (Button(TAC_ICON_FILE, "Create new tilemap."))
  {
    ShowAddMapDialog();
  }

  if (axis == ImGuiAxis_X)
  {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_OPEN, "Open tilemap."))
  {
    ShowOpenMapDialog();
  }

  if (axis == ImGuiAxis_X)
  {
    ImGui::SameLine();
  }
  ImGui::Spacing();

  if (axis == ImGuiAxis_X)
  {
    ImGui::SameLine();
  }
  if (Button(TAC_ICON_UNDO, "Undo", model.CanUndo()))
  {
    dispatcher.enqueue<UndoEvent>();
  }

  if (axis == ImGuiAxis_X)
  {
    ImGui::SameLine();
  }
  if (Button(TAC_ICON_REDO, "Redo", model.CanRedo()))
  {
    dispatcher.enqueue<RedoEvent>();
  }

  if (axis == ImGuiAxis_X)
  {
    ImGui::SameLine();
  }
  ImGui::Spacing();

  if (axis == ImGuiAxis_X)
  {
    ImGui::SameLine();
  }
  if (Button(TAC_ICON_TILESET, "Create tileset."))
  {
    ShowTilesetDialog();
  }

  if (axis == ImGuiAxis_X)
  {
    ImGui::SameLine();
  }
  if (Button(TAC_ICON_RESIZE, "Resize map."))
  {}

  if (axis == ImGuiAxis_X)
  {
    ImGui::SameLine();
  }
  ImGui::Spacing();

  if (axis == ImGuiAxis_X)
  {
    ImGui::SameLine();
  }
  if (ToolButton(TAC_ICON_STAMP, "Stamp tool.", model.IsStampActive()))
  {
    dispatcher.enqueue<SelectToolEvent>(MouseToolType::Stamp);
  }

  if (axis == ImGuiAxis_X)
  {
    ImGui::SameLine();
  }
  if (ToolButton(TAC_ICON_BUCKET, "Bucket tool.", model.IsBucketActive()))
  {
    dispatcher.enqueue<SelectToolEvent>(MouseToolType::Bucket);
  }

  if (axis == ImGuiAxis_X)
  {
    ImGui::SameLine();
  }
  if (ToolButton(TAC_ICON_ERASER, "Eraser tool.", model.IsEraserActive()))
  {
    dispatcher.enqueue<SelectToolEvent>(MouseToolType::Eraser);
  }

  EndDockingToolbar();
}

}  // namespace Tactile
