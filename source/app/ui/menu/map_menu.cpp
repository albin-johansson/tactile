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

#include "editor/app_context.hpp"
#include "editor/shortcut/mappings.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/model.hpp"
#include "ui/dialog/dialog_state.hpp"
#include "ui/dialog/dialogs.hpp"
#include "ui/dialog/godot_export_dialog.hpp"
#include "ui/dock/tileset/dialogs/create_tileset_dialog.hpp"
#include "ui/widget/menu_item.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {

void update_map_menu()
{
  const auto& model = get_model();
  const auto& lang = get_current_language();

  Disable disable {!model.is_map_active()};
  if (Menu menu {lang.menu.map.c_str()}; menu.is_open()) {
    menu_item(MenuAction::InspectMap);

    ImGui::Separator();

    menu_item(MenuAction::AddTileset, TACTILE_PRIMARY_MOD "+T");

    ImGui::Separator();

    menu_item(MenuAction::AddRow, TACTILE_SECONDARY_MOD "+R");
    menu_item(MenuAction::AddColumn, TACTILE_SECONDARY_MOD "+C");
    menu_item(MenuAction::RemoveRow, TACTILE_SECONDARY_MOD "+Shift+R");
    menu_item(MenuAction::RemoveColumn, TACTILE_SECONDARY_MOD "+Shift+C");

    ImGui::Separator();

    menu_item(MenuAction::FixInvalidTiles);
    // lazy_tooltip("##FixInvalidTilesTooltip", lang.tooltip.fix_invalid_tiles.c_str());

    ImGui::Separator();

    menu_item(MenuAction::ResizeMap);

    ImGui::Separator();

    if (Menu export_menu {lang.menu.export_as.c_str()}; export_menu.is_open()) {
      menu_item(MenuAction::ExportGodotScene);
    }
  }
}

void show_tileset_creation_dialog()
{
  get_dialogs().create_tileset.open();
}

}  // namespace tactile::ui
