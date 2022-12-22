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

#include "core/tile/tileset_bundle.hpp"
#include "debug/assert.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/document_events.hpp"
#include "model/event/property_events.hpp"
#include "model/event/tileset_events.hpp"
#include "model/model.hpp"
#include "tileset_view.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

constexpr auto tab_bar_flags =
    ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

void update_context_menu(const DocumentModel& model,
                         const UUID& tileset_id,
                         entt::dispatcher& dispatcher)
{
  if (auto popup = Popup::for_item("##TilesetTabContext"); popup.is_open()) {
    const auto& lang = get_current_language();

    if (ImGui::MenuItem(lang.action.create_tileset.c_str())) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.inspect_tileset.c_str())) {
      dispatcher.enqueue<InspectContextEvent>(tileset_id);
    }

    ImGui::Separator();

    if (Disable disable_if {model.is_open(tileset_id)};
        ImGui::MenuItem(lang.action.open_tileset.c_str())) {
      dispatcher.enqueue<OpenDocumentEvent>(tileset_id);
      dispatcher.enqueue<SelectDocumentEvent>(tileset_id);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.remove_tileset.c_str())) {
      dispatcher.enqueue<RemoveTilesetEvent>(tileset_id);
    }
  }
}

}  // namespace

void update_tileset_tabs(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(model.is_map_active());

  if (TabBar bar {"##TilesetTabBar", tab_bar_flags}; bar.is_open()) {
    if (ImGui::TabItemButton(TAC_ICON_ADD "##AddTilesetButton",
                             ImGuiTabItemFlags_Trailing)) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    const auto& map_document = model.require_active_map();
    const auto& map = map_document.get_map();
    const auto& tilesets = map.tileset_bundle();

    for (const auto& [tileset_id, ref]: tilesets) {
      const Scope scope {tileset_id};

      const auto& name = ref.view_tileset().ctx().name();
      const auto is_active = tilesets.active_tileset_id() == tileset_id;

      bool opened = true;
      if (TabItem item {name.c_str(),
                        &opened,
                        is_active ? ImGuiTabItemFlags_SetSelected : 0};
          item.is_open()) {
        update_tileset_view(model, tileset_id, dispatcher);
      }

      if (!opened) {
        dispatcher.enqueue<RemoveTilesetEvent>(tileset_id);
      }
      else if (ImGui::IsItemActivated()) {
        dispatcher.enqueue<SelectTilesetEvent>(tileset_id);
      }
      else {
        update_context_menu(model, tileset_id, dispatcher);
      }
    }
  }
}

}  // namespace tactile::ui
