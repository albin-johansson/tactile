#include "toolbar.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/model.hpp"
#include "core/tool_type.hpp"
#include "gui/events/command_events.hpp"
#include "gui/events/map_events.hpp"
#include "gui/events/save_events.hpp"
#include "gui/events/tool_events.hpp"
#include "gui/events/viewport_events.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/common/button.hpp"
#include "gui/widgets/common/docking_toolbar.hpp"
#include "gui/widgets/menus/file_menu.hpp"
#include "gui/widgets/menus/map_menu.hpp"
#include "gui/widgets/toolbar/tool_button.hpp"

namespace Tactile {
namespace {

constinit bool is_visible = true;
constinit bool has_focus = false;

}  // namespace

void UpdateToolbarWidget(const Model& model, entt::dispatcher& dispatcher)
{
  if (!is_visible) {
    return;
  }

  static int axis = ImGuiAxis_X;
  BeginDockingToolbar("Toolbar", axis);
  has_focus = ImGui::IsWindowFocused();

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

  if (Button(TAC_ICON_FILE, "Create new map")) {
    ShowAddMapDialog();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_OPEN, "Open map")) {
    ShowOpenMapDialog();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_SAVE, "Save", !model.IsClean())) {
    dispatcher.enqueue<SaveEvent>();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_UNDO, "Undo", model.CanUndo())) {
    dispatcher.enqueue<UndoEvent>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_REDO, "Redo", model.CanRedo())) {
    dispatcher.enqueue<RedoEvent>();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_CENTER, "Center viewport")) {
    dispatcher.enqueue<CenterViewportEvent>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_RESIZE, "Resize map")) {
    dispatcher.enqueue<OpenResizeMapDialogEvent>();
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (Button(TAC_ICON_TILESET, "Create tileset")) {
    ShowTilesetDialog();
  }

  separate();

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_STAMP, "Stamp tool", model.IsStampActive())) {
    dispatcher.enqueue<SelectToolEvent>(ToolType::Stamp);
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_BUCKET, "Bucket tool", model.IsBucketActive())) {
    dispatcher.enqueue<SelectToolEvent>(ToolType::Bucket);
  }

  if (axis == ImGuiAxis_X) {
    ImGui::SameLine();
  }

  if (ToolButton(TAC_ICON_ERASER, "Eraser tool", model.IsEraserActive())) {
    dispatcher.enqueue<SelectToolEvent>(ToolType::Eraser);
  }

  EndDockingToolbar();
}

void SetToolbarVisible(const bool visible) noexcept
{
  is_visible = visible;
}

auto IsToolbarVisible() noexcept -> bool
{
  return is_visible;
}

auto IsToolbarFocused() noexcept -> bool
{
  return has_focus;
}

}  // namespace Tactile
