// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "document_tab_widget.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "common/util/fmt.hpp"
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
            show_map_viewport(model, model.get_map_document(document_id), dispatcher);
          }
          if (model.is_tileset(document_id)) {
            show_tileset_viewport(model.get_tileset_document(document_id), dispatcher);
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
