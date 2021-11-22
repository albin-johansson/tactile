#include "edit_menu.hpp"

#include <imgui.h>

#include "core/tool_type.hpp"
#include "core/utils/formatted_string.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/widgets/common/menu.hpp"
#include "editor/gui/widgets/dialogs/settings_dialog.hpp"
#include "editor/model.hpp"

namespace Tactile {
namespace {

constinit bool show_settings_window = false;

}  // namespace

void UpdateEditMenu(const Model& model, entt::dispatcher& dispatcher)
{
  if (auto menu = Menu{"Edit"}) {
    const auto hasActiveDocument = model.HasActiveDocument();

    const auto canUndo = model.CanUndo();
    const auto canRedo = model.CanRedo();

    const FormattedString undoText{TAC_ICON_UNDO " Undo {}",
                                   canUndo ? model.GetUndoText() : ""};
    const FormattedString redoText{TAC_ICON_REDO " Redo {}",
                                   canRedo ? model.GetRedoText() : ""};

    if (ImGui::MenuItem(undoText.GetData(), "Ctrl+Z", false, canUndo)) {
      dispatcher.enqueue<UndoEvent>();
    }

    if (ImGui::MenuItem(redoText.GetData(), "Ctrl+Y", false, canRedo)) {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_STAMP " Stamp",
                        "S",
                        model.IsStampActive(),
                        hasActiveDocument))
    {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Stamp);
    }

    if (ImGui::MenuItem(TAC_ICON_BUCKET " Bucket",
                        "B",
                        model.IsBucketActive(),
                        hasActiveDocument))
    {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Bucket);
    }

    if (ImGui::MenuItem(TAC_ICON_ERASER " Eraser",
                        "E",
                        model.IsEraserActive(),
                        hasActiveDocument))
    {
      dispatcher.enqueue<SelectToolEvent>(ToolType::Eraser);
    }

    if (ImGui::MenuItem(TAC_ICON_OBJECT_SELECTION " Object Selection",
                        GetObjectSelectionToolShortcutLabel(),
                        model.IsObjectSelectionActive(),
                        model.IsObjectSelectionPossible()))
    {
      dispatcher.enqueue<SelectToolEvent>(ToolType::ObjectSelection);
    }

    ImGui::Separator();

    show_settings_window =
        ImGui::MenuItem(TAC_ICON_SETTINGS " Settings...", "Ctrl+Alt+S");
  }
}

void UpdateEditMenuWindows(entt::dispatcher& dispatcher)
{
  if (show_settings_window) {
    OpenSettingsDialog();
    show_settings_window = false;
  }

  UpdateSettingsDialog(dispatcher);
}

void ShowSettingsDialog() noexcept
{
  show_settings_window = true;
}

}  // namespace Tactile
