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

#include "tileset_tabs.hpp"

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/common/ecs.hpp"
#include "core/components/attributes.hpp"
#include "core/components/tiles.hpp"
#include "core/documents/tileset_document.hpp"
#include "core/events/document_events.hpp"
#include "core/events/property_events.hpp"
#include "core/events/tileset_events.hpp"
#include "core/model.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/tilesets/tileset_view.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {
namespace {

constexpr auto _tab_bar_flags =
    ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

void _update_context_menu(const DocumentModel& model,
                          const UUID& documentId,
                          const entt::entity tilesetEntity,
                          entt::dispatcher& dispatcher)
{
  if (auto popup = Popup::for_item("##TilesetTabContext"); popup.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_ADD " Create New Tileset...")) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Tileset")) {
      dispatcher.enqueue<InspectContextEvent>(tilesetEntity);
    }

    ImGui::Separator();

    {
      Disable disable;
      if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Tileset")) {
        // TODO
      }
    }

    ImGui::Separator();

    {
      Disable disable{model.is_open(documentId)};
      if (ImGui::MenuItem(TAC_ICON_OPEN " Open Tileset")) {
        dispatcher.enqueue<OpenDocumentEvent>(documentId);
        dispatcher.enqueue<SelectDocumentEvent>(documentId);
      }
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Tileset")) {
      dispatcher.enqueue<RemoveTilesetEvent>(documentId);
    }
  }
}

}  // namespace

void update_tileset_tabs(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(model.is_map_active());

  if (TabBar bar{"##TilesetTabBar", _tab_bar_flags}; bar.is_open()) {
    if (ImGui::TabItemButton(TAC_ICON_ADD "##AddTilesetButton",
                             ImGuiTabItemFlags_Trailing)) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    const auto& mapRegistry = model.get_active_registry();

    const auto& activeTileset = ctx_get<comp::ActiveTileset>(mapRegistry);
    for (auto&& [entity, ref] : mapRegistry.view<comp::TilesetRef>().each()) {
      const Scope scope{static_cast<int>(hash(ref.source_tileset))};

      const auto isActive = activeTileset.entity == entity;

      const auto& document = model.view_tileset(ref.source_tileset);
      const auto& name = document.get_name();

      bool opened = true;
      if (TabItem item{name.c_str(),
                       &opened,
                       isActive ? ImGuiTabItemFlags_SetSelected : 0};
          item.is_open()) {
        update_tileset_view(model, ref.source_tileset, dispatcher);
      }

      if (!opened) {
        dispatcher.enqueue<RemoveTilesetEvent>(ref.source_tileset);
      }
      else if (ImGui::IsItemActivated()) {
        dispatcher.enqueue<SelectTilesetEvent>(ref.source_tileset);
      }
      else {
        _update_context_menu(model, ref.source_tileset, entity, dispatcher);
      }
    }
  }
}

}  // namespace tactile::ui
