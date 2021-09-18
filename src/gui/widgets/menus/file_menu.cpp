#include "file_menu.hpp"

#include <imgui.h>
#include <portable-file-dialogs.h>

#include <utility>  // move

#include "core/model.hpp"
#include "events/map_events.hpp"
#include "events/quit_event.hpp"
#include "events/save_events.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/dialogs/add_map_dialog.hpp"
#include "gui/widgets/dialogs/settings_dialog.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

constinit bool show_settings_window = false;
constinit bool show_add_map_dialog = false;
constinit bool show_open_map_dialog = false;

void ShowMapFileDialog(entt::dispatcher& dispatcher)
{
  auto path =
      pfd::open_file{"Open Map...", "", {"Map Files", "*.json *.tmx *.xml"}}.result();

  if (!path.empty()) {
    dispatcher.enqueue<OpenMapEvent>(std::move(path.front()));
  }

  show_open_map_dialog = false;
}

}  // namespace

void UpdateFileMenu(const Model& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMenu("File")) {
    const auto hasActiveDocument = model.HasActiveDocument();

    show_add_map_dialog = ImGui::MenuItem(TAC_ICON_FILE " New map...", "Ctrl+N");
    show_open_map_dialog = ImGui::MenuItem(TAC_ICON_OPEN " Open map...", "Ctrl+O");

    // TODO "Recent files..." + Reopen last closed + Clear history

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_SAVE " Save", "Ctrl+S", false, model.CanSaveDocument()))
    {
      dispatcher.enqueue<SaveEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_SAVE " Save as...",
                        "Ctrl+Shift+S",
                        false,
                        hasActiveDocument))
    {
      dispatcher.enqueue<OpenSaveAsDialogEvent>();
    }

    ImGui::Separator();

    show_settings_window =
        ImGui::MenuItem(TAC_ICON_SETTINGS " Settings...", "Ctrl+Alt+S");

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_CLOSE " Close map", nullptr, false, hasActiveDocument)) {
      dispatcher.enqueue<CloseMapEvent>(model.GetActiveMapId().value());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_EXIT " Exit")) {
      dispatcher.enqueue<QuitEvent>();
    }

    ImGui::EndMenu();
  }
}

void UpdateFileMenuWindows(entt::dispatcher& dispatcher)
{
  if (show_add_map_dialog) {
    OpenAddMapDialog();
    show_add_map_dialog = false;
  }

  if (show_settings_window) {
    OpenSettingsDialog();
    show_settings_window = false;
  }

  UpdateAddMapDialog(dispatcher);
  UpdateSettingsDialog(dispatcher);

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

void ShowSettingsDialog() noexcept
{
  show_settings_window = true;
}

}  // namespace Tactile
