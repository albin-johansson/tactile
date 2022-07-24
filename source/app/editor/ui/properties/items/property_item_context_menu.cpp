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

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/event/property_events.hpp"
#include "editor/lang/language.hpp"
#include "editor/lang/strings.hpp"
#include "editor/ui/scoped.hpp"

namespace tactile::ui {

auto property_item_context_menu(const UUID&                   context_id,
                                entt::dispatcher&             dispatcher,
                                const std::string&            name,
                                PropertyItemContextMenuState& state) -> bool
{
  const auto& lang = get_current_language();

  if (auto popup = Popup::for_item("##PropertyItemPopup"); popup.is_open()) {
    state.show_rename_dialog = ImGui::MenuItem(lang.action.rename_property.c_str());
    state.show_change_type_dialog =
        ImGui::MenuItem(lang.action.change_property_type.c_str());

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.remove_property.c_str())) {
      dispatcher.enqueue<RemovePropertyEvent>(context_id, name);
    }

    return true;
  }
  else {
    return false;
  }
}

}  // namespace tactile::ui
