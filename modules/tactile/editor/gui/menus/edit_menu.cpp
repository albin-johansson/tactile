#include "edit_menu.hpp"

#include <imgui.h>

#include "core/tool_type.hpp"
#include "core/utils/formatted_string.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/gui/dialogs/settings_dialog.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"

namespace tactile {

void EditMenu::Update(const Model& model, entt::dispatcher& dispatcher)
{
  if (Scoped::Menu menu{"Edit"}; menu.IsOpen()) {
    const auto canUndo = model.CanUndo();
    const auto canRedo = model.CanRedo();

    const FormattedString undoText{TAC_ICON_UNDO " Undo {}",
                                   canUndo ? model.GetUndoText() : ""};
    const FormattedString redoText{TAC_ICON_REDO " Redo {}",
                                   canRedo ? model.GetRedoText() : ""};

    if (ImGui::MenuItem(undoText.GetData(), TACTILE_PRIMARY_MOD "+Z", false, canUndo)) {
      dispatcher.enqueue<UndoEvent>();
    }

    if (ImGui::MenuItem(redoText.GetData(), TACTILE_PRIMARY_MOD "+Y", false, canRedo)) {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_STAMP " Stamp",
                        "S",
                        model.IsStampActive(),
                        model.IsStampPossible())) {
      dispatcher.enqueue<SelectToolEvent>(tool_type::stamp);
    }

    if (ImGui::MenuItem(TAC_ICON_BUCKET " Bucket",
                        "B",
                        model.IsBucketActive(),
                        model.IsBucketPossible())) {
      dispatcher.enqueue<SelectToolEvent>(tool_type::bucket);
    }

    if (ImGui::MenuItem(TAC_ICON_ERASER " Eraser",
                        "E",
                        model.IsEraserActive(),
                        model.IsEraserPossible())) {
      dispatcher.enqueue<SelectToolEvent>(tool_type::eraser);
    }

    if (ImGui::MenuItem(TAC_ICON_OBJECT_SELECTION " Object Selection",
                        "Q",
                        model.IsObjectSelectionActive(),
                        model.IsObjectSelectionPossible())) {
      dispatcher.enqueue<SelectToolEvent>(tool_type::object_selection);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_COMPONENT " Component Editor...",
                        nullptr,
                        false,
                        model.HasActiveDocument())) {
      mComponentEditor.Open(model);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_SETTINGS " Settings...", TACTILE_PRIMARY_MOD "+,")) {
      mSettingsDialog.Open();
    }
  }
}

void EditMenu::UpdateWindows(const Model& model, entt::dispatcher& dispatcher)
{
  mSettingsDialog.Update(model, dispatcher);
  mComponentEditor.Update(model, dispatcher);
}

void EditMenu::OpenSettingsModal()
{
  mSettingsDialog.Open();
}

void EditMenu::ShowComponentEditor(const Model& model)
{
  mComponentEditor.Open(model);
}

}  // namespace tactile
