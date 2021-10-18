#include "file_menu.hpp"

#include <utility>  // move

#include <imgui.h>

#include "tactile/editor/events/map_events.hpp"
#include "tactile/editor/events/quit_event.hpp"
#include "tactile/editor/events/save_events.hpp"
#include "tactile/editor/gui/icons.hpp"
#include "tactile/editor/gui/widgets/common/menu.hpp"
#include "tactile/editor/gui/widgets/dialogs/add_map_dialog.hpp"
#include "tactile/editor/model.hpp"
#include "tactile/io/history.hpp"

#include <portable-file-dialogs.h>

namespace Tactile {
namespace {

constinit bool show_add_map_dialog = false;
constinit bool show_open_map_dialog = false;

void ShowMapFileDialog(entt::dispatcher& dispatcher)
{
  auto path = pfd::open_file{"Open Map...",  //
                             "",
                             {"Map Files", "*.json *.tmx *.xml *.yaml *.yml"}}
                  .result();

  if (!path.empty()) {
    dispatcher.enqueue<OpenMapEvent>(std::move(path.front()));
  }

  show_open_map_dialog = false;
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

    show_add_map_dialog = ImGui::MenuItem(TAC_ICON_FILE " New Map...", "Ctrl+N");
    show_open_map_dialog = ImGui::MenuItem(TAC_ICON_OPEN " Open Map...", "Ctrl+O");

    UpdateRecentFilesMenu(dispatcher);

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_SAVE " Save", "Ctrl+S", false, model.CanSaveDocument()))
    {
      dispatcher.enqueue<SaveEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_SAVE " Save As...",
                        "Ctrl+Shift+S",
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
  if (show_add_map_dialog) {
    OpenAddMapDialog();
    show_add_map_dialog = false;
  }

  UpdateAddMapDialog(dispatcher);

  if (show_open_map_dialog) {
    ShowMapFileDialog(dispatcher);
  }
}

void ShowAddMapDialog() noexcept
{
  show_add_map_dialog = true;
}

void ShowOpenMapDialog() noexcept
{
  show_open_map_dialog = true;
}

}  // namespace Tactile
