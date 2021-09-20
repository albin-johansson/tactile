#include "edit_menu.hpp"

#include <imgui.h>

#include <array>   // array
#include <format>  // format_to_n

#include "core/model.hpp"
#include "core/tool_type.hpp"
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
  if (ImGui::BeginMenu("Edit")) {
    const auto canUndo = model.CanUndo();
    const auto canRedo = model.CanRedo();

    // Zero-initialize buffers to ensure null-termination
    std::array<char, 128> undoText{};
    std::array<char, 128> redoText{};

    if (canUndo) {
      std::format_to_n(undoText.data(),
                       undoText.size(),
                       TAC_ICON_UNDO " Undo {}",
                       model.GetUndoText());
    }
    else {
      std::format_to_n(undoText.data(), undoText.size(), TAC_ICON_UNDO " Undo");
    }

    if (canRedo) {
      std::format_to_n(redoText.data(),
                       redoText.size(),
                       TAC_ICON_REDO " Redo {}",
                       model.GetRedoText());
    }
    else {
      std::format_to_n(redoText.data(), redoText.size(), TAC_ICON_REDO " Redo");
    }

    if (ImGui::MenuItem(undoText.data(), "Ctrl+Z", false, canUndo)) {
      dispatcher.enqueue<UndoEvent>();
    }

    if (ImGui::MenuItem(redoText.data(), "Ctrl+Y", false, canRedo)) {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Add row", "Alt+R", false, hasActiveDocument)) {
      dispatcher.enqueue<AddRowEvent>();
    }

    if (ImGui::MenuItem("Add column", "Alt+C", false, hasActiveDocument)) {
      dispatcher.enqueue<AddColumnEvent>();
    }

    if (ImGui::MenuItem("Remove row", "Alt+Shift+R", false, hasActiveDocument)) {
      dispatcher.enqueue<RemoveRowEvent>();
    }

    if (ImGui::MenuItem("Remove column", "Alt+Shift+C", false, hasActiveDocument)) {
      dispatcher.enqueue<RemoveColumnEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Resize map", nullptr, false, hasActiveDocument)) {
      dispatcher.enqueue<OpenResizeMapDialogEvent>();
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
  if (show_tileset_dialog) {
    OpenTilesetDialog();
    show_tileset_dialog = false;
  }

  UpdateTilesetDialog(dispatcher);
}

void ShowTilesetDialog() noexcept
{
  show_tileset_dialog = true;
}

}  // namespace Tactile
