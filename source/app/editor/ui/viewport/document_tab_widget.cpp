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

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/commands/command_stack.hpp"
#include "core/events/document_events.hpp"
#include "core/model.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/viewport/map_viewport.hpp"
#include "editor/ui/viewport/tileset_viewport.hpp"

namespace tactile::ui {

void update_document_tabs(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (TabBar bar {"##DocumentTabs", ImGuiTabBarFlags_Reorderable}; bar.is_open()) {
    model.each([&](const UUID& documentId) {
      const Scope scope {static_cast<int>(hash(documentId))};

      const auto isActive = model.active_document_id() == documentId;

      const auto& document = model.view_document(documentId);
      const auto& name = document.get_name();

      ImGuiTabItemFlags flags = 0;
      if (isActive) {
        flags |= ImGuiTabItemFlags_SetSelected;

        if (!document.get_history().is_clean()) {
          flags |= ImGuiTabItemFlags_UnsavedDocument;
        }
      }

      bool opened = true;
      if (TabItem item {name.c_str(), &opened, flags}; item.is_open()) {
        if (isActive) {
          if (model.is_map(documentId)) {
            show_map_viewport(model, model.view_map(documentId), dispatcher);
          }
          if (model.is_tileset(documentId)) {
            show_tileset_viewport(model.view_tileset(documentId), dispatcher);
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

}  // namespace tactile::ui
