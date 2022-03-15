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

#include "document_tab_view.hpp"

#include <imgui.h>

#include "core/components/attributes.hpp"
#include "editor/events/map_events.hpp"
#include "editor/gui/scoped.hpp"
#include "map_view.hpp"
#include "editor/model.hpp"

namespace tactile {

void show_document_tab_view(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (scoped::TabBar bar{"##MapViewportTabBar", ImGuiTabBarFlags_Reorderable};
      bar.is_open()) {
    for (const auto& [id, document] : model) {
      const scoped::Id scope{id};

      ImGuiTabItemFlags flags = 0;
      const auto isActive = model.active_map_id() == id;

      if (isActive) {
        flags |= ImGuiTabItemFlags_SetSelected;

        if (!model.is_clean()) {
          flags |= ImGuiTabItemFlags_UnsavedDocument;
        }
      }

      const auto& context = document->registry.ctx<comp::AttributeContext>();
      bool opened = true;
      if (scoped::TabItem item{context.name.c_str(), &opened, flags}; item.is_open()) {
        if (isActive) {
          update_map_view(document->registry, dispatcher);
        }
      }

      if (!opened) {
        dispatcher.enqueue<CloseMapEvent>(id);
      }
      else if (ImGui::IsItemActivated()) {
        dispatcher.enqueue<SelectMapEvent>(id);
      }
    }
  }
}

}  // namespace tactile
