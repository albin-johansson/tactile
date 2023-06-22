/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "model/documents/document_system.hpp"
#include "model/menus/menu_components.hpp"
#include "model/systems/language_system.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {

void push_map_menu(const Model& model, Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  const Disable disable_if {!sys::is_map_document_active(model)};
  if (const Menu menu {strings.menu.map.c_str()}; menu.is_open()) {
    push_menu_item(model, MenuAction::InspectMap, dispatcher);

    ImGui::Separator();

    push_menu_item(model, MenuAction::CreateTileset, dispatcher);

    ImGui::Separator();

    push_menu_item(model, MenuAction::AddRow, dispatcher);
    push_menu_item(model, MenuAction::AddColumn, dispatcher);
    push_menu_item(model, MenuAction::RemoveRow, dispatcher);
    push_menu_item(model, MenuAction::RemoveColumn, dispatcher);

    ImGui::Separator();

    push_menu_item(model, MenuAction::FixInvalidTiles, dispatcher);

    ImGui::Separator();

    push_menu_item(model, MenuAction::ResizeMap, dispatcher);

    ImGui::Separator();

    if (const Menu export_menu {strings.menu.export_as.c_str()}; export_menu.is_open()) {
      push_menu_item(model, MenuAction::ExportGodotScene, dispatcher);
    }
  }
}

}  // namespace tactile::ui
