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

#include "document_tab_widget.hpp"

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/commands/command_stack.hpp"
#include "core/components/attributes.hpp"
#include "core/events/document_events.hpp"
#include "core/model.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/gui/viewport/map_view.hpp"
#include "editor/gui/viewport/tileset_view.hpp"

namespace tactile {

void update_document_tabs(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (scoped::TabBar bar{"##DocumentTabs", ImGuiTabBarFlags_Reorderable}; bar.is_open()) {
    model.each([&](const UUID& documentId) {
      const scoped::Id scope{static_cast<int>(hash(documentId))};

      ImGuiTabItemFlags flags = 0;
      const auto isActive = model.active_document_id() == documentId;
      const auto document = model.get_document(documentId);

      if (isActive) {
        flags |= ImGuiTabItemFlags_SetSelected;

        if (!document->get_history().is_clean()) {
          flags |= ImGuiTabItemFlags_UnsavedDocument;
        }
      }

      const auto& registry = document->get_registry();
      const auto& context = ctx_get<comp::AttributeContext>(registry);

      bool opened = true;
      if (scoped::TabItem item{context.name.c_str(), &opened, flags}; item.is_open()) {
        if (isActive) {
          if (model.is_map(documentId)) {
            update_map_view(model, dispatcher);
          }
          if (model.is_tileset(documentId)) {
            update_tileset_view(model, dispatcher);
          }
        }
      }

      if (!opened) {
        dispatcher.enqueue<CloseDocumentEvent>(documentId);
      }
      else if (ImGui::IsItemActivated()) {
        dispatcher.enqueue<SelectDocumentEvent>(documentId);
      }
    });
  }
}

}  // namespace tactile
