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

#include "tileset_tabs.hpp"

#include <imgui.h>

#include "common/debug/assert.hpp"
#include "model/contexts/context_components.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/file_events.hpp"
#include "model/events/property_events.hpp"
#include "model/events/tileset_events.hpp"
#include "model/i18n/language_system.hpp"
#include "model/maps/map_components.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "ui/dock/tileset/tileset_view.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

void _push_context_menu(const Registry& registry,
                        const Strings& strings,
                        const Entity tileset_document_entity,
                        const Entity attached_tileset_entity,
                        Dispatcher& dispatcher)
{
  if (auto popup = Popup::for_item("##TilesetTabContext"); popup.is_open()) {
    if (ImGui::MenuItem(strings.action.create_tileset.c_str())) {
      dispatcher.enqueue<ShowNewTilesetDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.inspect_tileset.c_str())) {
      dispatcher.enqueue<InspectContextEvent>(attached_tileset_entity);
    }

    ImGui::Separator();

    if (Disable disable_if {sys::is_document_open(registry, tileset_document_entity)};
        ImGui::MenuItem(strings.action.open_tileset.c_str())) {
      dispatcher.enqueue<OpenDocumentEvent>(tileset_document_entity);
      dispatcher.enqueue<SelectDocumentEvent>(tileset_document_entity);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.remove_tileset.c_str())) {
      dispatcher.enqueue<DetachTilesetEvent>(sys::get_active_map(registry),
                                             attached_tileset_entity);
    }
  }
}

}  // namespace

void push_tileset_tabs(const Registry& registry, Dispatcher& dispatcher)
{
  TACTILE_ASSERT(sys::is_map_document_active(registry));

  constexpr ImGuiTabBarFlags tab_bar_flags =
      ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

  if (const TabBar bar {"##TilesetTabBar", tab_bar_flags}; bar.is_open()) {
    if (ImGui::TabItemButton(TAC_ICON_ADD "##AddTilesetButton",
                             ImGuiTabItemFlags_Trailing)) {
      dispatcher.enqueue<ShowNewTilesetDialogEvent>();
    }

    const auto& strings = sys::get_current_language_strings(registry);
    const auto map_document_entity = sys::get_active_document(registry);

    const auto& map_document = registry.get<MapDocument>(map_document_entity);
    const auto& map = registry.get<Map>(map_document.map);

    for (const auto attached_tileset_entity: map.attached_tilesets) {
      const Scope scope {attached_tileset_entity};
      const auto& attached_tileset =
          registry.get<AttachedTileset>(attached_tileset_entity);

      const auto tileset_entity = attached_tileset.tileset;
      const auto tileset_document_entity =
          sys::get_associated_tileset_document(registry, tileset_entity);
      TACTILE_ASSERT(tileset_document_entity != kNullEntity);

      const auto tileset_name = sys::get_document_name(registry, tileset_document_entity);
      const auto is_tileset_active = map.active_tileset == attached_tileset_entity;

      if (const TabItem item {tileset_name.c_str(),
                              nullptr,
                              is_tileset_active ? ImGuiTabItemFlags_SetSelected : 0};
          item.is_open()) {
        update_attached_tileset_view(registry, attached_tileset_entity, dispatcher);
      }

      if (ImGui::IsItemClicked(ImGuiMouseButton_Left) ||
          ImGui::IsItemClicked(ImGuiMouseButton_Right) || ImGui::IsItemActivated()) {
        dispatcher.enqueue<SelectTilesetEvent>(attached_tileset_entity);
      }
      else {
        _push_context_menu(registry,
                           strings,
                           tileset_document_entity,
                           attached_tileset_entity,
                           dispatcher);
      }
    }
  }
}

}  // namespace tactile::ui
