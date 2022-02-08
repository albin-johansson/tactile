#include "map_menu.hpp"

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/gui/tilesets/dialogs/create_tileset_dialog.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"

namespace tactile {

MapMenu::MapMenu() : mCreateTilesetDialog{std::make_unique<CreateTilesetDialog>()} {}

MapMenu::~MapMenu() noexcept = default;

void MapMenu::Update(const Model& model, entt::dispatcher& dispatcher)
{
  scoped::disable disable{!model.has_active_document()};
  if (scoped::menu menu{"Map"}; menu.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Map")) {
      dispatcher.enqueue<inspect_map_event>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_TILESET " Add Tileset...", TACTILE_PRIMARY_MOD "+T")) {
      ShowAddTilesetDialog();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Add Row", TACTILE_SECONDARY_MOD "+R")) {
      dispatcher.enqueue<add_row_event>();
    }

    if (ImGui::MenuItem("Add Column", TACTILE_SECONDARY_MOD "+C")) {
      dispatcher.enqueue<add_column_event>();
    }

    if (ImGui::MenuItem("Remove Row", TACTILE_SECONDARY_MOD "+Shift+R")) {
      dispatcher.enqueue<remove_row_event>();
    }

    if (ImGui::MenuItem("Remove Column", TACTILE_SECONDARY_MOD "+Shift+C")) {
      dispatcher.enqueue<remove_column_event>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Resize Map...")) {
      dispatcher.enqueue<open_resize_map_dialog_event>();
    }
  }

  mCreateTilesetDialog->update(model, dispatcher);
}

void MapMenu::ShowAddTilesetDialog()
{
  mCreateTilesetDialog->Open();
}

}  // namespace tactile
