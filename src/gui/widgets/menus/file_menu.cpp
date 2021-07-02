#include "file_menu.hpp"

#include <IconsFontAwesome5.h>

#include "core/events/add_map_event.hpp"
#include "core/events/open_map_event.hpp"
#include "core/events/quit_event.hpp"
#include "core/model.hpp"
#include "gui/widgets/dialogs/settings_dialog.hpp"
#include "gui/widgets/file_dialog.hpp"
#include "imgui.h"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

inline bool show_settings_window = false;
inline bool show_map_file_dialog = false;

[[nodiscard]] auto GetFilter() -> czstring
{
  if (Prefs::GetPreferredFormat() == "JSON")
  {
    return ".json,.tmx";
  }
  else
  {
    return ".tmx,.json";
  }
}

void ShowMapFileDialog(entt::dispatcher& dispatcher)
{
  const auto result = FileDialog("MapFileDialog", "Open map...", GetFilter());
  if (result == FileDialogResult::Success)
  {
    dispatcher.enqueue<OpenMapEvent>(GetFileDialogSelectedPath());
    show_map_file_dialog = false;
  }
  else if (result == FileDialogResult::Close)
  {
    show_map_file_dialog = false;
  }
}

}  // namespace

void UpdateFileMenu(const Model& model, entt::dispatcher& dispatcher)
{
  const auto* document = model.GetActiveDocument();
  if (ImGui::BeginMenu("File"))
  {
    if (ImGui::MenuItem(ICON_FA_FILE " New map...", "Ctrl+N"))
    {
      dispatcher.enqueue<AddMapEvent>();
    }

    show_map_file_dialog =
        ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open map...", "Ctrl+O");

    if (ImGui::MenuItem("Close map", nullptr, false, document != nullptr))
    {}

    ImGui::Separator();

    if (ImGui::MenuItem(ICON_FA_SAVE " Save", "Ctrl+S"))
    {}

    if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
    {}

    ImGui::Separator();

    show_settings_window =
        ImGui::MenuItem(ICON_FA_COG " Settings...", "Ctrl+Alt+S");

    ImGui::Separator();

    if (ImGui::MenuItem(ICON_FA_WINDOW_CLOSE " Exit"))
    {
      dispatcher.enqueue<QuitEvent>();
    }

    ImGui::EndMenu();
  }
}

void UpdateFileMenuWindows(entt::dispatcher& dispatcher)
{
  if (show_settings_window)
  {
    UpdateSettingsDialog(&show_settings_window);
  }

  if (show_map_file_dialog)
  {
    ShowMapFileDialog(dispatcher);
  }
}

void EnableOpenMapDialog() noexcept
{
  show_map_file_dialog = true;
}

void EnableSettingsDialog() noexcept
{
  show_settings_window = true;
}

}  // namespace Tactile
