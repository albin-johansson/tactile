#include "file_menu.hpp"

#include <imgui.h>

#include "core/model.hpp"
#include "events/maps/open_map_event.hpp"
#include "events/quit_event.hpp"
#include "events/save_as_request_event.hpp"
#include "events/save_event.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/common/file_dialog.hpp"
#include "gui/widgets/dialogs/add_map_dialog.hpp"
#include "gui/widgets/dialogs/settings_dialog.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

inline bool show_settings_window = false;
inline bool show_add_map_dialog = false;
inline bool show_open_map_dialog = false;

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
  const auto result = FileDialogImport("MapFileDialog", "Open map...", GetFilter());
  if (result == FileDialogResult::Success)
  {
    dispatcher.enqueue<OpenMapEvent>(GetFileDialogSelectedPath());
    show_open_map_dialog = false;
  }
  else if (result == FileDialogResult::Close)
  {
    show_open_map_dialog = false;
  }
}

}  // namespace

void UpdateFileMenu(const Model& model, entt::dispatcher& dispatcher)
{
  const auto* document = model.GetActiveDocument();
  if (ImGui::BeginMenu("File"))
  {
    show_add_map_dialog = ImGui::MenuItem(TAC_ICON_FILE " New map...", "Ctrl+N");
    show_open_map_dialog = ImGui::MenuItem(TAC_ICON_OPEN " Open map...", "Ctrl+O");

    if (ImGui::MenuItem("Close map", nullptr, false, document != nullptr))
    {}

    ImGui::Separator();

    // FIXME the behaviour of the clean state is not 100% accurate
    if (ImGui::MenuItem(TAC_ICON_SAVE " Save",
                        "Ctrl+S",
                        false,
                        document && (!document->HasPath() || !document->IsClean())))
    {
      dispatcher.enqueue<SaveEvent>();
    }

    if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S", false, document != nullptr))
    {
      dispatcher.enqueue<SaveAsRequestEvent>();
    }

    ImGui::Separator();

    show_settings_window =
        ImGui::MenuItem(TAC_ICON_SETTINGS " Settings...", "Ctrl+Alt+S");

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_EXIT " Exit"))
    {
      dispatcher.enqueue<QuitEvent>();
    }

    ImGui::EndMenu();
  }
}

void UpdateFileMenuWindows(entt::dispatcher& dispatcher)
{
  if (show_add_map_dialog)
  {
    OpenAddMapDialog();
    show_add_map_dialog = false;
  }

  UpdateAddMapDialog(dispatcher);

  if (show_settings_window)
  {
    UpdateSettingsDialog(dispatcher, &show_settings_window);
  }

  if (show_open_map_dialog)
  {
    ShowMapFileDialog(dispatcher);
  }
}

void EnableAddMapDialog() noexcept
{
  show_add_map_dialog = true;
}

void EnableOpenMapDialog() noexcept
{
  show_open_map_dialog = true;
}

void EnableSettingsDialog() noexcept
{
  show_settings_window = true;
}

}  // namespace Tactile
