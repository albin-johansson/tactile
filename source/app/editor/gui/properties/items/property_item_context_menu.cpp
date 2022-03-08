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

#include "property_item_context_menu.hpp"

#include <imgui.h>

#include "editor/events/property_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace tactile {

auto PropertyItemContextMenu(entt::dispatcher& dispatcher,
                             const std::string& name,
                             PropertyItemContextMenuState& state) -> bool
{
  if (auto popup = scoped::popup::for_item("##PropertyItemContext"); popup.is_open()) {
    state.show_rename_dialog = ImGui::MenuItem(TAC_ICON_EDIT " Rename Property...");
    state.show_change_type_dialog = ImGui::MenuItem(ICON_FA_SHAPES " Change Type...");

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Property")) {
      dispatcher.enqueue<remove_property_event>(name);
    }

    return true;
  }
  else {
    return false;
  }
}

}  // namespace tactile
