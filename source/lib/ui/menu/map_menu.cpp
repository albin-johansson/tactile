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

#include "model/events/menu_events.hpp"
#include "model/model_view.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile {

void push_map_menu(ModelView& model)
{
  const auto& strings = model.get_language_strings();

  if (const ui::Menu menu {strings.menu.map.c_str(), model.has_active_map_document()};
      menu.is_open()) {
    if (ImGui::MenuItem(strings.action.inspect_map.c_str(),
                        nullptr,
                        nullptr,
                        model.is_available(MenuAction::InspectMap))) {
      model.enqueue<MenuActionEvent>(MenuAction::InspectMap);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.create_tileset.c_str(),
                        TACTILE_PRIMARY_MOD "+T",
                        nullptr,
                        model.is_available(MenuAction::CreateTileset))) {
      model.enqueue<MenuActionEvent>(MenuAction::CreateTileset);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.add_row.c_str(),
                        TACTILE_SECONDARY_MOD "+R",
                        nullptr,
                        model.is_available(MenuAction::AddRow))) {
      model.enqueue<MenuActionEvent>(MenuAction::AddRow);
    }

    if (ImGui::MenuItem(strings.action.add_column.c_str(),
                        TACTILE_SECONDARY_MOD "+C",
                        nullptr,
                        model.is_available(MenuAction::AddColumn))) {
      model.enqueue<MenuActionEvent>(MenuAction::AddColumn);
    }

    if (ImGui::MenuItem(strings.action.remove_row.c_str(),
                        TACTILE_SECONDARY_MOD "+Shift+R",
                        nullptr,
                        model.is_available(MenuAction::RemoveRow))) {
      model.enqueue<MenuActionEvent>(MenuAction::RemoveRow);
    }

    if (ImGui::MenuItem(strings.action.remove_column.c_str(),
                        TACTILE_SECONDARY_MOD "+Shift+C",
                        nullptr,
                        model.is_available(MenuAction::RemoveColumn))) {
      model.enqueue<MenuActionEvent>(MenuAction::RemoveColumn);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.fix_invalid_tiles.c_str(),
                        nullptr,
                        nullptr,
                        model.is_available(MenuAction::FixInvalidTiles))) {
      model.enqueue<MenuActionEvent>(MenuAction::FixInvalidTiles);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.resize_map.c_str(),
                        nullptr,
                        nullptr,
                        model.is_available(MenuAction::ResizeMap))) {
      model.enqueue<MenuActionEvent>(MenuAction::ResizeMap);
    }

    ImGui::Separator();

    if (const ui::Menu export_menu {strings.menu.export_as.c_str()};
        export_menu.is_open()) {
      if (ImGui::MenuItem(strings.action.export_as_godot_scene.c_str(),
                          nullptr,
                          nullptr,
                          model.is_available(MenuAction::ExportGodotScene))) {
        model.enqueue<MenuActionEvent>(MenuAction::ExportGodotScene);
      }
    }
  }
}

}  // namespace tactile
