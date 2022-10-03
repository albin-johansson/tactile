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

#include "core/event/map_events.hpp"
#include "core/model.hpp"
#include "editor/shortcut/mappings.hpp"
#include "editor/ui/common/tooltips.hpp"
#include "editor/ui/dialog/godot_export_dialog.hpp"
#include "editor/ui/dock/tileset/dialogs/create_tileset_dialog.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/shared/dialog_state.hpp"
#include "editor/ui/shared/dialogs.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::ui {

void update_map_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  Disable disable {!model.is_map_active()};
  if (Menu menu {lang.menu.map.c_str()}; menu.is_open()) {
    if (ImGui::MenuItem(lang.action.inspect_map.c_str())) {
      dispatcher.enqueue<InspectMapEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.add_tileset.c_str(), TACTILE_PRIMARY_MOD "+T")) {
      show_tileset_creation_dialog();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.add_row.c_str(), TACTILE_SECONDARY_MOD "+R")) {
      dispatcher.enqueue<AddRowEvent>();
    }

    if (ImGui::MenuItem(lang.action.add_column.c_str(), TACTILE_SECONDARY_MOD "+C")) {
      dispatcher.enqueue<AddColumnEvent>();
    }

    if (ImGui::MenuItem(lang.action.remove_row.c_str(),
                        TACTILE_SECONDARY_MOD "+Shift+R")) {
      dispatcher.enqueue<RemoveRowEvent>();
    }

    if (ImGui::MenuItem(lang.action.remove_column.c_str(),
                        TACTILE_SECONDARY_MOD "+Shift+C")) {
      dispatcher.enqueue<RemoveColumnEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.fix_invalid_tiles.c_str())) {
      dispatcher.enqueue<FixTilesInMapEvent>();
    }

    lazy_tooltip("##FixInvalidTilesTooltip", lang.tooltip.fix_invalid_tiles.c_str());

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.resize_map.c_str())) {
      dispatcher.enqueue<OpenResizeMapDialogEvent>();
    }

    ImGui::Separator();

    if (Menu export_menu {lang.menu.export_as.c_str()}; export_menu.is_open()) {
      if (ImGui::MenuItem(lang.action.export_as_godot_scene.c_str())) {
        get_dialogs().godot_export.open();
      }
    }
  }

  get_dialogs().create_tileset.update(model, dispatcher);
  get_dialogs().godot_export.update(model, dispatcher);
}

void show_tileset_creation_dialog()
{
  get_dialogs().create_tileset.open();
}

}  // namespace tactile::ui
