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

#include "document_tab_widget.hpp"

#include <imgui.h>

#include "cmd/command_stack.hpp"
#include "common/fmt/fmt_string.hpp"
#include "components/document.hpp"
#include "model/event/file_events.hpp"
#include "model/systems/document_system.hpp"
#include "ui/style/icons.hpp"
#include "ui/viewport/map_viewport.hpp"
#include "ui/viewport/tileset_viewport.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

void _push_document_tab(const Model& model,
                        const Entity document_entity,
                        Dispatcher& dispatcher)
{
  const Scope scope {document_entity};

  const auto& document = model.get<Document>(document_entity);
  const auto& command_stack = model.get<CommandStack>(document_entity);

  const char* document_icon =
      (document.type == DocumentType::Map) ? TAC_ICON_MAP : TAC_ICON_TILESET;
  const auto document_name = sys::get_document_name(model, document_entity);
  const FmtString<256> name_with_icon {"{} {}", document_icon, document_name};

  ImGuiTabItemFlags flags = 0;

  const auto is_active = sys::get_active_document(model) == document_entity;
  if (is_active) {
    flags |= ImGuiTabItemFlags_SetSelected;

    if (!command_stack.is_clean()) {
      flags |= ImGuiTabItemFlags_UnsavedDocument;
    }
  }

  bool opened = true;
  if (const TabItem item {name_with_icon.data(), &opened, flags}; item.is_open()) {
    if (is_active) {
      if (model.has<MapDocument>(document_entity)) {
        show_map_viewport(model, document_entity, dispatcher);
      }

      if (model.has<TilesetDocument>(document_entity)) {
        show_tileset_viewport(model, document_entity, dispatcher);
      }
    }
  }

  if (!opened) {
    dispatcher.enqueue<CloseDocumentEvent>(document_entity);
  }
  else if (ImGui::IsItemActivated()) {
    dispatcher.enqueue<SelectDocumentEvent>(document_entity);
  }
}

}  // namespace

void update_document_tabs(const Model& model, Dispatcher& dispatcher)
{
  if (const TabBar bar {"##DocumentTabs", ImGuiTabBarFlags_Reorderable}; bar.is_open()) {
    const auto& document_context = model.get<DocumentContext>();
    for (const auto document_entity: document_context.open_documents) {
      _push_document_tab(model, document_entity, dispatcher);
    }
  }
}

}  // namespace tactile::ui
