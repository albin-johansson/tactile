#include "map_menu.hpp"

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/common/menu.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/gui/tilesets/dialogs/tileset_dialog.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"

namespace Tactile {

void MapMenu::Update(const Model& model, entt::dispatcher& dispatcher)
{
  Scoped::Disable disable{!model.HasActiveDocument()};

  Menu menu{"Map"};
  if (menu) {
    if (ImGui::MenuItem(TAC_ICON_PROPERTIES " Show Map Properties")) {
      dispatcher.enqueue<ShowMapPropertiesEvent>();
    }

    ImGui::Separator();

    mShowTilesetDialog =
        ImGui::MenuItem(TAC_ICON_TILESET " Add Tileset...", TACTILE_PRIMARY_MOD "+T");

    ImGui::Separator();

    if (ImGui::MenuItem("Add Row", TACTILE_SECONDARY_MOD "+R")) {
      dispatcher.enqueue<AddRowEvent>();
    }

    if (ImGui::MenuItem("Add Column", TACTILE_SECONDARY_MOD "+C")) {
      dispatcher.enqueue<AddColumnEvent>();
    }

    if (ImGui::MenuItem("Remove Row", TACTILE_SECONDARY_MOD "+Shift+R")) {
      dispatcher.enqueue<RemoveRowEvent>();
    }

    if (ImGui::MenuItem("Remove Column", TACTILE_SECONDARY_MOD "+Shift+C")) {
      dispatcher.enqueue<RemoveColumnEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Resize Map...")) {
      dispatcher.enqueue<OpenResizeMapDialogEvent>();
    }
  }
}

void MapMenu::UpdateWindows(entt::dispatcher& dispatcher)
{
  if (mShowTilesetDialog) {
    OpenTilesetDialog();
    mShowTilesetDialog = false;
  }

  UpdateTilesetDialog(dispatcher);
}

void MapMenu::ShowAddTilesetDialog()
{
  mShowTilesetDialog = true;
}

}  // namespace Tactile
