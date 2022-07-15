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

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/document/map_document.hpp"
#include "core/document/tileset_document.hpp"
#include "core/event/document_events.hpp"
#include "core/event/property_events.hpp"
#include "core/event/tileset_events.hpp"
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
                          const UUID&          tilesetId,
                          entt::dispatcher&    dispatcher)
{
  if (auto popup = Popup::for_item("##TilesetTabContext"); popup.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_ADD " Create New Tileset...")) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Tileset")) {
      dispatcher.enqueue<InspectContextEvent>(tilesetId);
    }

    ImGui::Separator();

    if (Disable disableIf {model.is_open(tilesetId)};
        ImGui::MenuItem(TAC_ICON_OPEN " Open Tileset")) {
      dispatcher.enqueue<OpenDocumentEvent>(tilesetId);
      dispatcher.enqueue<SelectDocumentEvent>(tilesetId);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Tileset")) {
      dispatcher.enqueue<RemoveTilesetEvent>(tilesetId);
    }
  }
}

}  // namespace

void update_tileset_tabs(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(model.is_map_active());

  if (TabBar bar {"##TilesetTabBar", _tab_bar_flags}; bar.is_open()) {
    if (ImGui::TabItemButton(TAC_ICON_ADD "##AddTilesetButton",
                             ImGuiTabItemFlags_Trailing)) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    const auto& mapDocument = model.require_active_map();
    const auto& map = mapDocument.get_map();
    const auto& tilesets = map.get_tilesets();

    for (const auto& [tilesetId, ref] : tilesets) {
      const Scope scope {tilesetId};

      const auto& name = ref.view_tileset().get_name();
      const auto  isActive = tilesets.active_tileset_id() == tilesetId;

      bool opened = true;
      if (TabItem item {name.c_str(),
                        &opened,
                        isActive ? ImGuiTabItemFlags_SetSelected : 0};
          item.is_open()) {
        update_tileset_view(model, tilesetId, dispatcher);
      }

      if (!opened) {
        dispatcher.enqueue<RemoveTilesetEvent>(tilesetId);
      }
      else if (ImGui::IsItemActivated()) {
        dispatcher.enqueue<SelectTilesetEvent>(tilesetId);
      }
      else {
        _update_context_menu(model, tilesetId, dispatcher);
      }
    }
  }
}

}  // namespace tactile::ui
