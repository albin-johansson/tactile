#include "map_menu.hpp"

#include <imgui.h>

#include "core/model.hpp"
#include "events/map_events.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/common/menu.hpp"
#include "gui/widgets/tilesets/dialogs/tileset_dialog.hpp"

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

    show_tileset_dialog = ImGui::MenuItem(TAC_ICON_TILESET " Add Tileset...", "Ctrl+T");

    ImGui::Separator();

    if (ImGui::MenuItem("Add Row", "Alt+R")) {
      dispatcher.enqueue<AddRowEvent>();
    }

    if (ImGui::MenuItem("Add Column", "Alt+C")) {
      dispatcher.enqueue<AddColumnEvent>();
    }

    if (ImGui::MenuItem("Remove Row", "Alt+Shift+R")) {
      dispatcher.enqueue<RemoveRowEvent>();
    }

    if (ImGui::MenuItem("Remove Column", "Alt+Shift+C")) {
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
