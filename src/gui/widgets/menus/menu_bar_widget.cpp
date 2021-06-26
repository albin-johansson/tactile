#include "menu_bar_widget.hpp"

#include <IconsFontAwesome5.h>

#include <centurion.hpp>  // is_debug_build

#include "core/events/add_map_event.hpp"
#include "core/events/center_viewport_event.hpp"
#include "core/events/open_map_event.hpp"
#include "core/events/quit_event.hpp"
#include "core/events/redo_event.hpp"
#include "core/events/undo_event.hpp"
#include "core/model.hpp"
#include "gui/layout/dock_space.hpp"
#include "gui/widgets/dialogs/about_dialog.hpp"
#include "gui/widgets/dialogs/settings_dialog.hpp"
#include "gui/widgets/dialogs/tileset_dialog.hpp"
#include "gui/widgets/file_dialog.hpp"
#include "gui/widgets/viewport/viewport_widget.hpp"
#include "imgui.h"

namespace tactile {
namespace {

inline bool show_about_tactile_window = false;
inline bool show_about_imgui_window = false;
inline bool show_metrics_window = false;
inline bool show_demo_window = false;
inline bool show_settings_window = false;
inline bool show_map_file_dialog = false;
inline bool show_tileset_dialog = false;

inline bool is_grid_item_toggled = true;

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

void ShowEditMenu(const Model& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMenu("Edit"))
  {
    if (ImGui::MenuItem(ICON_FA_UNDO " Undo", "Ctrl+Z"))
    {
      dispatcher.enqueue<UndoEvent>();
    }

    if (ImGui::MenuItem(ICON_FA_REDO " Redo", "Ctrl+Y"))
    {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Add row", "Alt+R"))
    {}

    if (ImGui::MenuItem("Add column", "Alt+C"))
    {}

    if (ImGui::MenuItem("Remove row", "Alt+Shift+R"))
    {}

    if (ImGui::MenuItem("Remove column", "Alt+Shift+C"))
    {}

    ImGui::Separator();

    if (ImGui::MenuItem(ICON_FA_STAMP " Stamp", "S"))
    {}

    if (ImGui::MenuItem(ICON_FA_FILL " Bucket", "B"))
    {}

    if (ImGui::MenuItem(ICON_FA_ERASER " Eraser", "E"))
    {}

    ImGui::Separator();

    show_tileset_dialog =
        ImGui::MenuItem(ICON_FA_IMAGE " Create tileset...", "Ctrl+T");

    ImGui::EndMenu();
  }
}

void ShowViewAppearanceSubmenu(const Model& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMenu("Widgets"))
  {
    if (ImGui::MenuItem("Reset layout"))
    {
      ResetLayout();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Mouse tools", nullptr, true))
    {}

    if (ImGui::MenuItem("Properties", nullptr, true))
    {}

    if (ImGui::MenuItem("Layers", nullptr, true))
    {}

    if (ImGui::MenuItem("Tilesets", nullptr, true))
    {}

    ImGui::EndMenu();
  }
}

void ShowViewMenu(const Model& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMenu("View"))
  {
    ShowViewAppearanceSubmenu(model, dispatcher);

    ImGui::Separator();

    if (ImGui::MenuItem(ICON_FA_CROSSHAIRS " Center viewport", "Ctrl+Space"))
    {
      dispatcher.enqueue<CenterViewportEvent>();
    }

    if (ImGui::MenuItem(ICON_FA_TH " Toggle grid",
                        "Ctrl+G",
                        &is_grid_item_toggled))
    {
      SetViewportGridEnabled(is_grid_item_toggled);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(ICON_FA_SEARCH_PLUS " Increase zoom", "Ctrl+Plus"))
    {}

    if (ImGui::MenuItem(ICON_FA_SEARCH_MINUS " Decrease zoom", "Ctrl+Minus"))
    {}

    if (ImGui::MenuItem(ICON_FA_SEARCH " Reset zoom"))
    {}

    ImGui::Separator();

    if (ImGui::MenuItem(ICON_FA_ARROW_UP " Pan up", "Ctrl+Up"))
    {}

    if (ImGui::MenuItem(ICON_FA_ARROW_DOWN " Pan down", "Ctrl+Down"))
    {}

    if (ImGui::MenuItem(ICON_FA_ARROW_RIGHT " Pan right", "Ctrl+Right"))
    {}

    if (ImGui::MenuItem(ICON_FA_ARROW_LEFT " Pan left", "Ctrl+Left"))
    {}

    ImGui::EndMenu();
  }
}

void ShowHelpMenu()
{
  if (ImGui::BeginMenu("Help"))
  {
    show_about_tactile_window =
        ImGui::MenuItem(ICON_FA_QUESTION_CIRCLE " About Tactile...");
    show_about_imgui_window =
        ImGui::MenuItem(ICON_FA_QUESTION_CIRCLE " About ImGui...");

    ImGui::Separator();

    show_metrics_window =
        ImGui::MenuItem(ICON_FA_TACHOMETER_ALT " Show metrics...");

    if constexpr (cen::is_debug_build())
    {
      ImGui::Separator();
      show_demo_window = ImGui::MenuItem("Show demo window...");
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
    ShowEditMenu(model, dispatcher);
    ShowViewMenu(model, dispatcher);
    ShowHelpMenu();

    ImGui::EndMainMenuBar();
  }

  if (show_settings_window)
  {
    UpdateSettingsDialog(&show_settings_window);
  }

  if (show_about_tactile_window)
  {
    UpdateAboutDialog(&show_about_tactile_window);
  }

  if (show_about_imgui_window)
  {
    ImGui::ShowAboutWindow(&show_about_imgui_window);
  }

  if (show_metrics_window)
  {
    ImGui::ShowMetricsWindow(&show_metrics_window);
  }

  if (show_map_file_dialog)
  {
    ShowMapFileDialog(dispatcher);
  }

  if (show_tileset_dialog)
  {
    UpdateTilesetDialog(&show_tileset_dialog, dispatcher);
  }

  if constexpr (cen::is_debug_build())
  {
    if (show_demo_window)
    {
      ImGui::ShowDemoWindow(&show_demo_window);
    }
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

void EnableTilesetDialog()
{
  show_tileset_dialog = true;
}

void ToggleMapGrid()
{
  is_grid_item_toggled = !is_grid_item_toggled;
  SetViewportGridEnabled(is_grid_item_toggled);
}

}  // namespace tactile
