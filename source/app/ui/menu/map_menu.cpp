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

#include "app/app_context.hpp"
#include "editor/shortcut/mappings.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/model.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {

void update_map_menu()
{
  const auto& model = get_model();
  const auto& lang = get_current_language();

  const Disable disable {!model.is_map_active()};
  if (const Menu menu {lang.menu.map.c_str()}; menu.is_open()) {
    ui_menu_item(MenuAction::InspectMap);

    ImGui::Separator();

    ui_menu_item(MenuAction::AddTileset, TACTILE_PRIMARY_MOD "+T");

    ImGui::Separator();

    ui_menu_item(MenuAction::AddRow, TACTILE_SECONDARY_MOD "+R");
    ui_menu_item(MenuAction::AddColumn, TACTILE_SECONDARY_MOD "+C");
    ui_menu_item(MenuAction::RemoveRow, TACTILE_SECONDARY_MOD "+Shift+R");
    ui_menu_item(MenuAction::RemoveColumn, TACTILE_SECONDARY_MOD "+Shift+C");

    ImGui::Separator();

    ui_menu_item(MenuAction::FixInvalidTiles);
    // ui_lazy_tooltip("##FixInvalidTilesTooltip", lang.tooltip.fix_invalid_tiles.c_str());

    ImGui::Separator();

    ui_menu_item(MenuAction::ResizeMap);

    ImGui::Separator();

    if (const Menu export_menu {lang.menu.export_as.c_str()}; export_menu.is_open()) {
      ui_menu_item(MenuAction::ExportGodotScene);
    }
  }
}

}  // namespace tactile::ui
