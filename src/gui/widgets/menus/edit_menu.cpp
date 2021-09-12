#include "edit_menu.hpp"

#include <imgui.h>

#include <format>  // format

#include "core/model.hpp"
#include "core/mouse_tool_type.hpp"
#include "events/command_events.hpp"
#include "events/map_events.hpp"
#include "events/tool_events.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/tilesets/dialogs/tileset_dialog.hpp"

namespace Tactile {
namespace {

constinit bool show_tileset_dialog = false;

}  // namespace

void UpdateEditMenu(const Model& model, entt::dispatcher& dispatcher)
{
  const auto hasActiveDocument = model.HasActiveDocument();
  if (ImGui::BeginMenu("Edit"))
  {
    const auto canUndo = model.CanUndo();
    const auto undoText =
        canUndo ? std::format(TAC_ICON_UNDO " Undo {}", model.GetUndoText())
                : std::string{TAC_ICON_UNDO " Undo"};
    if (ImGui::MenuItem(undoText.c_str(), "Ctrl+Z", false, canUndo))
    {
      dispatcher.enqueue<UndoEvent>();
    }

    const auto canRedo = model.CanRedo();
    const auto redoText =
        canRedo ? std::format(TAC_ICON_REDO " Redo {}", model.GetRedoText())
                : std::string{TAC_ICON_REDO " Redo"};
    if (ImGui::MenuItem(redoText.c_str(), "Ctrl+Y", false, canRedo))
    {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Add row", "Alt+R", false, hasActiveDocument))
    {
      dispatcher.enqueue<AddRowEvent>();
    }

    if (ImGui::MenuItem("Add column", "Alt+C", false, hasActiveDocument))
    {
      dispatcher.enqueue<AddColumnEvent>();
    }

    if (ImGui::MenuItem("Remove row", "Alt+Shift+R", false, hasActiveDocument))
    {
      dispatcher.enqueue<RemoveRowEvent>();
    }

    if (ImGui::MenuItem("Remove column", "Alt+Shift+C", false, hasActiveDocument))
    {
      dispatcher.enqueue<RemoveColumnEvent>();
    }

    if (ImGui::MenuItem("Resize map", nullptr, false, hasActiveDocument))
    {
      dispatcher.enqueue<OpenResizeMapDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_STAMP " Stamp",
                        "S",
                        model.IsStampActive(),
                        hasActiveDocument))
    {
      dispatcher.enqueue<SelectToolEvent>(MouseToolType::Stamp);
    }

    if (ImGui::MenuItem(TAC_ICON_BUCKET " Bucket",
                        "B",
                        model.IsBucketActive(),
                        hasActiveDocument))
    {
      dispatcher.enqueue<SelectToolEvent>(MouseToolType::Bucket);
    }

    if (ImGui::MenuItem(TAC_ICON_ERASER " Eraser",
                        "E",
                        model.IsEraserActive(),
                        hasActiveDocument))
    {
      dispatcher.enqueue<SelectToolEvent>(MouseToolType::Eraser);
    }

    ImGui::Separator();

    show_tileset_dialog = ImGui::MenuItem(TAC_ICON_TILESET " Create tileset...",
                                          "Ctrl+T",
                                          false,
                                          hasActiveDocument);

    ImGui::EndMenu();
  }
}

void UpdateEditMenuWindows(entt::dispatcher& dispatcher)
{
  if (show_tileset_dialog)
  {
    UpdateTilesetDialog(&show_tileset_dialog, dispatcher);
  }
}

void ShowTilesetDialog() noexcept
{
  show_tileset_dialog = true;
}

}  // namespace Tactile
