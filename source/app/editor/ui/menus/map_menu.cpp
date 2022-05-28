/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "map_menu.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/events/map_events.hpp"
#include "core/model.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/tilesets/dialogs/create_tileset_dialog.hpp"

namespace tactile::ui {
namespace {

struct MapMenuState final
{
  CreateTilesetDialog tileset_creation_dialog;
};

[[nodiscard]] auto _get_state() -> MapMenuState&
{
  static MapMenuState state;
  return state;
}

}  // namespace

void update_map_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  scoped::Disable disable{!model.is_map_active()};
  if (scoped::Menu menu{"Map"}; menu.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Map")) {
      dispatcher.enqueue<InspectMapEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_TILESET " Add Tileset...", TACTILE_PRIMARY_MOD "+T")) {
      show_tileset_creation_dialog();
    }

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

  _get_state().tileset_creation_dialog.update(model, dispatcher);
}

void show_tileset_creation_dialog()
{
  _get_state().tileset_creation_dialog.open();
}

}  // namespace tactile::ui
