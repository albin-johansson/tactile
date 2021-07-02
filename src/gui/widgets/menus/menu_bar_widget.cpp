#include "menu_bar_widget.hpp"

#include <IconsFontAwesome5.h>

#include "core/events/add_map_event.hpp"
#include "core/events/open_map_event.hpp"
#include "core/events/quit_event.hpp"
#include "core/events/select_tool_event.hpp"
#include "core/model.hpp"
#include "edit_menu.hpp"
#include "gui/widgets/dialogs/settings_dialog.hpp"
#include "gui/widgets/file_dialog.hpp"
#include "help_menu.hpp"
#include "imgui.h"
#include "view_menu.hpp"

namespace Tactile {
namespace {

inline bool show_settings_window = false;
inline bool show_map_file_dialog = false;

void ShowFileMenu(const Model& model, entt::dispatcher& dispatcher)
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

void ShowMapFileDialog(entt::dispatcher& dispatcher)
{
  const auto filter = ".json,.tmx";
  const auto result = FileDialog("MapFileDialogID", "Open map...", filter);
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

void UpdateMenuBarWidget(const Model& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMainMenuBar())
  {
    ShowFileMenu(model, dispatcher);
    UpdateEditMenu(model, dispatcher);
    UpdateViewMenu(model, dispatcher);
    UpdateHelpMenu();

    ImGui::EndMainMenuBar();
  }

  UpdateEditMenuWindows(dispatcher);
  UpdateHelpMenuWindows();

  if (show_settings_window)
  {
    UpdateSettingsDialog(&show_settings_window);
  }

  if (show_map_file_dialog)
  {
    ShowMapFileDialog(dispatcher);
  }
}

void EnableOpenMapDialog()
{
  show_map_file_dialog = true;
}

void EnableSettingsDialog()
{
  show_settings_window = true;
}

}  // namespace Tactile
