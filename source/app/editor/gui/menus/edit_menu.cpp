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
  if (scoped::menu menu{"Edit"}; menu.is_open()) {
    const auto canUndo = model.can_undo();
    const auto canRedo = model.can_redo();

    const formatted_string undoText{TAC_ICON_UNDO " Undo {}",
                                    canUndo ? model.get_undo_text() : ""};
    const formatted_string redoText{TAC_ICON_REDO " Redo {}",
                                    canRedo ? model.get_redo_text() : ""};

    if (ImGui::MenuItem(undoText.data(), TACTILE_PRIMARY_MOD "+Z", false, canUndo)) {
      dispatcher.enqueue<undo_event>();
    }

    if (ImGui::MenuItem(redoText.data(), TACTILE_PRIMARY_MOD "+Y", false, canRedo)) {
      dispatcher.enqueue<redo_event>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_STAMP " Stamp Tool",
                        "S",
                        model.is_tool_active(tool_type::stamp),
                        model.is_tool_possible(tool_type::stamp))) {
      dispatcher.enqueue<select_tool_event>(tool_type::stamp);
    }

    if (ImGui::MenuItem(TAC_ICON_BUCKET " Bucket Tool",
                        "B",
                        model.is_tool_active(tool_type::bucket),
                        model.is_tool_possible(tool_type::bucket))) {
      dispatcher.enqueue<select_tool_event>(tool_type::bucket);
    }

    if (ImGui::MenuItem(TAC_ICON_ERASER " Eraser Tool",
                        "E",
                        model.is_tool_active(tool_type::eraser),
                        model.is_tool_possible(tool_type::eraser))) {
      dispatcher.enqueue<select_tool_event>(tool_type::eraser);
    }

    if (ImGui::MenuItem(TAC_ICON_OBJECT_SELECTION " Object Selection Tool",
                        "Q",
                        model.is_tool_active(tool_type::object_selection),
                        model.is_tool_possible(tool_type::object_selection))) {
      dispatcher.enqueue<select_tool_event>(tool_type::object_selection);
    }

    if (ImGui::MenuItem(TAC_ICON_RECTANGLE " Rectangle Tool",
                        "R",
                        model.is_tool_active(tool_type::rectangle),
                        model.is_tool_possible(tool_type::rectangle))) {
      dispatcher.enqueue<select_tool_event>(tool_type::rectangle);
    }

    if (ImGui::MenuItem(TAC_ICON_ELLIPSE " Ellipse Tool",
                        "T",
                        model.is_tool_active(tool_type::ellipse),
                        model.is_tool_possible(tool_type::ellipse))) {
      dispatcher.enqueue<select_tool_event>(tool_type::ellipse);
    }

    if (ImGui::MenuItem(TAC_ICON_POINT " Point Tool",
                        "Y",
                        model.is_tool_active(tool_type::point),
                        model.is_tool_possible(tool_type::point))) {
      dispatcher.enqueue<select_tool_event>(tool_type::point);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_COMPONENT " Component Editor...",
                        nullptr,
                        false,
                        model.has_active_document())) {
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
