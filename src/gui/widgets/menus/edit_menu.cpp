#include "edit_menu.hpp"

#include <imgui.h>

#include <format>  // format

#include "core/events/maps/add_column_event.hpp"
#include "core/events/maps/add_row_event.hpp"
#include "core/events/maps/remove_column_event.hpp"
#include "core/events/maps/remove_row_event.hpp"
#include "core/events/redo_event.hpp"
#include "core/events/tools/select_tool_event.hpp"
#include "core/events/undo_event.hpp"
#include "core/model.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/tilesets/dialogs/tileset_dialog.hpp"

namespace Tactile {
namespace {

inline bool show_tileset_dialog = false;

}  // namespace

void UpdateEditMenu(const Model& model, entt::dispatcher& dispatcher)
{
  const auto* document = model.GetActiveDocument();
  const auto hasActiveMap = model.GetActiveMapId().has_value();
  if (ImGui::BeginMenu("Edit"))
  {
    const auto canUndo = document && document->CanUndo();
    const auto undoText =
        canUndo ? std::format(TAC_ICON_UNDO " Undo {}", document->GetUndoText())
                : std::string{TAC_ICON_UNDO " Undo"};
    if (ImGui::MenuItem(undoText.c_str(), "Ctrl+Z", false, canUndo))
    {
      dispatcher.enqueue<UndoEvent>();
    }

    const auto canRedo = document && document->CanRedo();
    const auto redoText =
        canRedo ? std::format(TAC_ICON_REDO " Redo {}", document->GetRedoText())
                : std::string{TAC_ICON_REDO " Redo"};
    if (ImGui::MenuItem(redoText.c_str(), "Ctrl+Y", false, canRedo))
    {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Add row", "Alt+R", false, hasActiveMap))
    {
      dispatcher.enqueue<AddRowEvent>();
    }

    if (ImGui::MenuItem("Add column", "Alt+C", false, hasActiveMap))
    {
      dispatcher.enqueue<AddColumnEvent>();
    }

    if (ImGui::MenuItem("Remove row", "Alt+Shift+R", false, hasActiveMap))
    {
      dispatcher.enqueue<RemoveRowEvent>();
    }

    if (ImGui::MenuItem("Remove column", "Alt+Shift+C", false, hasActiveMap))
    {
      dispatcher.enqueue<RemoveColumnEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_STAMP " Stamp",
                        "S",
                        model.IsStampActive(),
                        hasActiveMap))
    {
      dispatcher.enqueue<SelectToolEvent>(MouseToolType::Stamp);
    }

    if (ImGui::MenuItem(TAC_ICON_BUCKET " Bucket",
                        "B",
                        model.IsBucketActive(),
                        hasActiveMap))
    {
      dispatcher.enqueue<SelectToolEvent>(MouseToolType::Bucket);
    }

    if (ImGui::MenuItem(TAC_ICON_ERASER " Eraser",
                        "E",
                        model.IsEraserActive(),
                        hasActiveMap))
    {
      dispatcher.enqueue<SelectToolEvent>(MouseToolType::Eraser);
    }

    ImGui::Separator();

    show_tileset_dialog = ImGui::MenuItem(TAC_ICON_TILESET " Create tileset...",
                                          "Ctrl+T",
                                          false,
                                          hasActiveMap);

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

void EnableTilesetDialog() noexcept
{
  show_tileset_dialog = true;
}

}  // namespace Tactile
