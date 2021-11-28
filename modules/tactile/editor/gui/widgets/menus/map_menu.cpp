#include "map_menu.hpp"

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/widgets/common/menu.hpp"
#include "editor/gui/widgets/tilesets/dialogs/tileset_dialog.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"

namespace Tactile {
namespace {

constinit bool show_tileset_dialog = false;

}  // namespace

void UpdateMapMenu(const Model& model, entt::dispatcher& dispatcher)
{
  ImGui::BeginDisabled(!model.HasActiveDocument());

  if (auto menu = Menu{"Map"}) {
    if (ImGui::MenuItem(TAC_ICON_PROPERTIES " Show Map Properties")) {
      dispatcher.enqueue<ShowMapPropertiesEvent>();
    }

    ImGui::Separator();

    show_tileset_dialog =
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

  ImGui::EndDisabled();
}

void UpdateMapMenuWindows(entt::dispatcher& dispatcher)
{
  if (show_tileset_dialog) {
    OpenTilesetDialog();
    show_tileset_dialog = false;
  }

  UpdateTilesetDialog(dispatcher);
}

void ShowTilesetDialog() noexcept
{
  show_tileset_dialog = true;
}

}  // namespace Tactile
