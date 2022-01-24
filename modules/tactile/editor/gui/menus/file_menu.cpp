#include "file_menu.hpp"

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/events/quit_event.hpp"
#include "editor/events/save_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "io/file_dialog.hpp"
#include "io/history.hpp"

namespace tactile {

void FileMenu::Update(const Model& model, entt::dispatcher& dispatcher)
{
  if (scoped::Menu menu{"File"}; menu.IsOpen()) {
    const auto hasActiveDocument = model.HasActiveDocument();

    if (ImGui::MenuItem(TAC_ICON_FILE " Create Map...", TACTILE_PRIMARY_MOD "+N")) {
      mCreateMapDialog.Open();
    }

    mShowOpenMapDialog =
        ImGui::MenuItem(TAC_ICON_OPEN " Open Map...", TACTILE_PRIMARY_MOD "+O");

    UpdateRecentFilesMenu(dispatcher);

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_SAVE " Save",
                        TACTILE_PRIMARY_MOD "+S",
                        false,
                        model.CanSaveDocument())) {
      dispatcher.enqueue<SaveEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_SAVE " Save As...",
                        TACTILE_PRIMARY_MOD "+Shift+S",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<OpenSaveAsDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_CLOSE " Close Map", nullptr, false, hasActiveDocument)) {
      dispatcher.enqueue<CloseMapEvent>(model.GetActiveMapId().value());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_EXIT " Exit")) {
      dispatcher.enqueue<QuitEvent>();
    }
  }
}

void FileMenu::UpdateWindows(const Model& model, entt::dispatcher& dispatcher)
{
  mCreateMapDialog.Update(model, dispatcher);

  if (mShowOpenMapDialog) {
    UpdateMapFileDialog(dispatcher);
  }
}

void FileMenu::ShowNewMapDialog()
{
  mCreateMapDialog.Open();
}

void FileMenu::ShowOpenMapDialog()
{
  mShowOpenMapDialog = true;
}

void FileMenu::UpdateRecentFilesMenu(entt::dispatcher& dispatcher)
{
  if (scoped::Menu menu{TAC_ICON_HISTORY " Recent Files"}; menu.IsOpen()) {
    if (ImGui::MenuItem(TAC_ICON_OPEN " Reopen Last Closed File",
                        nullptr,
                        false,
                        HasValidLastClosedFile())) {
      // TODO this will need to be tweaked if tileset documents viewing will be supported
      dispatcher.enqueue<OpenMapEvent>(GetLastClosedFile());
    }

    const auto& history = GetFileHistory();

    if (!history.empty()) {
      ImGui::Separator();
    }

    for (const auto& path : history) {
      if (ImGui::MenuItem(path.c_str())) {
        /* It's fine if the file doesn't exist anymore, the parser handles that */
        dispatcher.enqueue<OpenMapEvent>(path);
      }
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_CLEAR_HISTORY " Clear File History",
                        nullptr,
                        false,
                        !history.empty())) {
      ClearFileHistory();
    }
  }
}

void FileMenu::UpdateMapFileDialog(entt::dispatcher& dispatcher)
{
  auto dialog = FileDialog::OpenMap();

  if (dialog.IsOkay()) {
    dispatcher.enqueue<OpenMapEvent>(dialog.GetPath());
  }

  mShowOpenMapDialog = false;
}

}  // namespace tactile
