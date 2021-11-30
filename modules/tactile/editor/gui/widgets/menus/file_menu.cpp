#include "file_menu.hpp"

#include <utility>  // move

#include <imgui.h>
#include <portable-file-dialogs.h>

#include "editor/events/map_events.hpp"
#include "editor/events/quit_event.hpp"
#include "editor/events/save_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/widgets/common/menu.hpp"
#include "editor/gui/widgets/dialogs/add_map_dialog.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "io/history.hpp"

namespace Tactile {
namespace {

constinit bool gShowAddMapDialog = false;
constinit bool gShowOpenMapDialog = false;

void ShowMapFileDialog(entt::dispatcher& dispatcher)
{
  auto path = pfd::open_file{"Open Map...",  //
                             "",
                             {"Map Files", "*.json *.tmx *.xml *.yaml *.yml"}}
                  .result();

  if (!path.empty()) {
    dispatcher.enqueue<OpenMapEvent>(std::move(path.front()));
  }

  gShowOpenMapDialog = false;
}

void UpdateRecentFilesMenu(entt::dispatcher& dispatcher)
{
  if (auto menu = Menu{TAC_ICON_HISTORY " Recent Files"}) {
    if (ImGui::MenuItem(TAC_ICON_OPEN " Reopen Last Closed File",
                        nullptr,
                        false,
                        HasValidLastClosedFile()))
    {
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
                        !history.empty()))
    {
      ClearFileHistory();
    }
  }
}

}  // namespace

void UpdateFileMenu(const Model& model, entt::dispatcher& dispatcher)
{
  if (auto menu = Menu{"File"}) {
    const auto hasActiveDocument = model.HasActiveDocument();

    gShowAddMapDialog =
        ImGui::MenuItem(TAC_ICON_FILE " New Map...", TACTILE_PRIMARY_MOD "+N");
    gShowOpenMapDialog =
        ImGui::MenuItem(TAC_ICON_OPEN " Open Map...", TACTILE_PRIMARY_MOD "+O");

    UpdateRecentFilesMenu(dispatcher);

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_SAVE " Save",
                        TACTILE_PRIMARY_MOD "+S",
                        false,
                        model.CanSaveDocument()))
    {
      dispatcher.enqueue<SaveEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_SAVE " Save As...",
                        TACTILE_PRIMARY_MOD "+Shift+S",
                        false,
                        hasActiveDocument))
    {
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

void UpdateFileMenuWindows(entt::dispatcher& dispatcher)
{
  if (gShowAddMapDialog) {
    OpenAddMapDialog();
    gShowAddMapDialog = false;
  }

  UpdateAddMapDialog(dispatcher);

  if (gShowOpenMapDialog) {
    ShowMapFileDialog(dispatcher);
  }
}

void ShowAddMapDialog() noexcept
{
  gShowAddMapDialog = true;
}

void ShowOpenMapDialog() noexcept
{
  gShowOpenMapDialog = true;
}

}  // namespace Tactile
