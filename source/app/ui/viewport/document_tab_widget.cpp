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

#include "core/util/fmt.hpp"
#include "model/cmd/command_stack.hpp"
#include "model/event/document_events.hpp"
#include "model/model.hpp"
#include "tileset_viewport.hpp"
#include "ui/style/icons.hpp"
#include "ui/viewport/map_viewport.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {

void update_document_tabs(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (const TabBar bar {"##DocumentTabs", ImGuiTabBarFlags_Reorderable}; bar.is_open()) {
    model.each([&](const UUID& document_id) {
      const Scope scope {document_id};

      const auto& document = model.get_document(document_id);
      const auto& document_name = document.get_name();
      const char* document_icon = document.is_map() ? TAC_ICON_MAP : TAC_ICON_TILESET;
      const FmtString<256> name_with_icon {"{} {}", document_icon, document_name};

      ImGuiTabItemFlags flags = 0;

      const auto is_active = model.get_active_document_id() == document_id;
      if (is_active) {
        flags |= ImGuiTabItemFlags_SetSelected;

        if (!document.get_history().is_clean()) {
          flags |= ImGuiTabItemFlags_UnsavedDocument;
        }
      }

      bool opened = true;
      if (const TabItem item {name_with_icon.data(), &opened, flags}; item.is_open()) {
        if (is_active) {
          if (model.is_map(document_id)) {
            show_map_viewport(model, model.get_map(document_id), dispatcher);
          }
          if (model.is_tileset(document_id)) {
            show_tileset_viewport(model.get_tileset(document_id), dispatcher);
          }
        }
      }

      if (!opened) {
        dispatcher.enqueue<CloseDocumentEvent>(document_id);
      }
      else if (ImGui::IsItemActivated()) {
        dispatcher.enqueue<SelectDocumentEvent>(document_id);
      }
    });
  }
}

}  // namespace tactile::ui
