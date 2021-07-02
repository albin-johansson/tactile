#include "menu_bar_widget.hpp"

#include <IconsFontAwesome5.h>

#include <centurion.hpp>  // is_debug_build

#include "core/events/add_column_event.hpp"
#include "core/events/add_map_event.hpp"
#include "core/events/add_row_event.hpp"
#include "core/events/center_viewport_event.hpp"
#include "core/events/open_map_event.hpp"
#include "core/events/quit_event.hpp"
#include "core/events/redo_event.hpp"
#include "core/events/remove_column_event.hpp"
#include "core/events/remove_row_event.hpp"
#include "core/events/select_tool_event.hpp"
#include "core/events/undo_event.hpp"
#include "core/model.hpp"
#include "gui/layout/dock_space.hpp"
#include "gui/widgets/dialogs/settings_dialog.hpp"
#include "gui/widgets/dialogs/tileset_dialog.hpp"
#include "gui/widgets/file_dialog.hpp"
#include "gui/widgets/layers/layer_widget.hpp"
#include "gui/widgets/properties/properties_widget.hpp"
#include "gui/widgets/tilesets/tileset_widget.hpp"
#include "help_menu.hpp"
#include "view_menu.hpp"
#include "imgui.h"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

inline bool show_settings_window = false;
inline bool show_map_file_dialog = false;
inline bool show_tileset_dialog = false;

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
  const auto* document = model.GetActiveDocument();
  const auto hasActiveMap = model.GetActiveMapId().has_value();
  if (ImGui::BeginMenu("Edit"))
  {
    if (ImGui::MenuItem(ICON_FA_UNDO " Undo",
                        "Ctrl+Z",
                        false,
                        document && document->CanUndo()))
    {
      dispatcher.enqueue<UndoEvent>();
    }

    if (ImGui::MenuItem(ICON_FA_REDO " Redo",
                        "Ctrl+Y",
                        false,
                        document && document->CanRedo()))
    {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Add row", "Alt+R", false, hasActiveMap))
    {
      dispatcher.enqueue<AddRowEvent>();
    }

    if (ImGui::MenuItem("Add column", "Alt+C", false, hasActiveMap))
    {
      dispatcher.enqueue<AddColumnEvent>();
    }

    if (ImGui::MenuItem("Remove row", "Alt+Shift+R", false, hasActiveMap))
    {
      dispatcher.enqueue<RemoveRowEvent>();
    }

    if (ImGui::MenuItem("Remove column", "Alt+Shift+C", false, hasActiveMap))
    {
      dispatcher.enqueue<RemoveColumnEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(ICON_FA_STAMP " Stamp",
                        "S",
                        model.IsStampActive(),
                        hasActiveMap))
    {
      dispatcher.enqueue<SelectToolEvent>(MouseToolType::Stamp);
    }

    if (ImGui::MenuItem(ICON_FA_FILL " Bucket",
                        "B",
                        model.IsBucketActive(),
                        hasActiveMap))
    {
      dispatcher.enqueue<SelectToolEvent>(MouseToolType::Bucket);
    }

    if (ImGui::MenuItem(ICON_FA_ERASER " Eraser",
                        "E",
                        model.IsEraserActive(),
                        hasActiveMap))
    {
      dispatcher.enqueue<SelectToolEvent>(MouseToolType::Eraser);
    }

    ImGui::Separator();

    show_tileset_dialog = ImGui::MenuItem(ICON_FA_IMAGE " Create tileset...",
                                          "Ctrl+T",
                                          false,
                                          hasActiveMap);

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
    UpdateViewMenu(model, dispatcher);
    UpdateHelpMenu();

    ImGui::EndMainMenuBar();
  }

  UpdateHelpMenuWindows();

  if (show_settings_window)
  {
    UpdateSettingsDialog(&show_settings_window);
  }

  if (show_map_file_dialog)
  {
    ShowMapFileDialog(dispatcher);
  }

  if (show_tileset_dialog)
  {
    UpdateTilesetDialog(&show_tileset_dialog, dispatcher);
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

}  // namespace Tactile
