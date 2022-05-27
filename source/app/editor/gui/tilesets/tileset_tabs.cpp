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
#include "editor/events/property_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/gui/tilesets/tileset_view.hpp"
#include "editor/model.hpp"

namespace tactile {
namespace {

constexpr auto _tab_bar_flags =
    ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

void _update_context_menu(const UUID& id,
                          const entt::entity tilesetEntity,
                          entt::dispatcher& dispatcher)
{
  if (auto popup = scoped::Popup::for_item("##TilesetTabContext"); popup.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_ADD " Create New Tileset...")) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Tileset")) {
      dispatcher.enqueue<InspectContextEvent>(tilesetEntity);
    }

    ImGui::Separator();

    {
      scoped::Disable disable;
      if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Tileset")) {
        // TODO
      }
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Tileset")) {
      dispatcher.enqueue<RemoveTilesetEvent>(id);
    }
  }
}

}  // namespace

void update_tileset_tabs(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (scoped::TabBar bar{"##TilesetTabBar", _tab_bar_flags}; bar.is_open()) {
    if (ImGui::TabItemButton(TAC_ICON_ADD "##AddTilesetButton",
                             ImGuiTabItemFlags_Trailing)) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    const auto& mapRegistry = model.get_active_registry();

    const auto& activeTileset = ctx_get<comp::ActiveTileset>(mapRegistry);
    for (auto&& [entity, ref] : mapRegistry.view<comp::TilesetRef>().each()) {
      //      const scoped::Id scope{ref.first_id};
      const scoped::Id scope{static_cast<int>(hash(ref.source_tileset))};

      const auto isActive = activeTileset.entity == entity;

      // FIXME context is invalid for tileset references
      const auto& context = checked_get<comp::AttributeContext>(mapRegistry, entity);

      bool opened = true;
      if (scoped::TabItem item{context.name.c_str(),
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
        _update_context_menu(context.id, entity, dispatcher);
      }
    }
  }
}

}  // namespace tactile
