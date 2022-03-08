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

void MapMenu::Update(const document_model& model, entt::dispatcher& dispatcher)
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
