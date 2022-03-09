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

#include <imgui.h>

#include "core/components/attributes.hpp"
#include "core/components/tiles.hpp"
#include "editor/events/property_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/menus/map_menu.hpp"
#include "editor/gui/scoped.hpp"
#include "tileset_view.hpp"

namespace tactile {
namespace {

constexpr auto gTabBarFlags =
    ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

void UpdateContextMenu(const tileset_id id,
                       const entt::entity tilesetEntity,
                       entt::dispatcher& dispatcher)
{
  if (auto popup = scoped::popup::for_item("##TilesetTabContext"); popup.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_ADD " Create New Tileset...")) {
      dispatcher.enqueue<show_add_tileset_dialog_event>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Tileset")) {
      dispatcher.enqueue<inspect_context_event>(tilesetEntity);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Tileset")) {
      // TODO
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Tileset")) {
      dispatcher.enqueue<remove_tileset_event>(id);
    }
  }
}

}  // namespace

void TilesetTabWidget::Update(const entt::registry& registry,
                              entt::dispatcher& dispatcher)
{
  if (scoped::tab_bar bar{"TilesetTabBar", gTabBarFlags}; bar.is_open()) {
    if (ImGui::TabItemButton(TAC_ICON_ADD "##AddTilesetButton",
                             ImGuiTabItemFlags_Trailing)) {
      dispatcher.enqueue<show_add_tileset_dialog_event>();
    }

    const auto& activeTileset = registry.ctx<comp::active_tileset>();
    for (auto&& [entity, tileset] : registry.view<comp::tileset>().each()) {
      const scoped::id scope{tileset.id};

      const auto isActive = activeTileset.entity == entity;
      const auto& context = registry.get<comp::AttributeContext>(entity);

      bool opened = true;
      if (scoped::tab_item item{context.name.c_str(),
                                &opened,
                                isActive ? ImGuiTabItemFlags_SetSelected : 0};
          item.is_open()) {
        mTilesetView.Update(registry, entity, dispatcher);
      }

      if (!opened) {
        dispatcher.enqueue<remove_tileset_event>(tileset.id);
      }
      else if (ImGui::IsItemActivated()) {
        dispatcher.enqueue<select_tileset_event>(tileset.id);
      }
      else {
        UpdateContextMenu(tileset.id, entity, dispatcher);
      }
    }
  }
}

auto TilesetTabWidget::GetTilesetView() const -> const TilesetView&
{
  return mTilesetView;
}

}  // namespace tactile
