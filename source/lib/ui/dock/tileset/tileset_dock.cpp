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

#include "tileset_dock.hpp"

#include <centurion/events/mouse_events.hpp>
#include <imgui.h>

#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/setting_events.hpp"
#include "model/events/tileset_events.hpp"
#include "model/events/viewport_events.hpp"
#include "model/i18n/language_system.hpp"
#include "model/maps/map_components.hpp"
#include "model/viewports/viewport_components.hpp"
#include "ui/dock/tileset/tileset_tabs.hpp"
#include "ui/style/alignment.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {

void push_tileset_dock_widget(const Registry& registry,
                              TilesetDockState& state,
                              Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(registry);
  const auto& settings = registry.get<Settings>();

  if (!settings.test_flag(SETTINGS_SHOW_TILESET_DOCK_BIT)) {
    return;
  }

  bool show_tileset_dock = true;
  const Window dock {strings.window.tileset_dock.c_str(),
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
                     &show_tileset_dock};

  if (show_tileset_dock != settings.test_flag(SETTINGS_SHOW_TILESET_DOCK_BIT)) {
    dispatcher.enqueue<SetFlagSettingEvent>(SETTINGS_SHOW_TILESET_DOCK_BIT,
                                            show_tileset_dock);
  }

  // We intentionally do not use the window is_hovered function here
  state.has_focus = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);
  state.has_hover = ImGui::IsWindowHovered(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    const auto document_entity = sys::get_active_document(registry);
    const auto& map_document = registry.get<MapDocument>(document_entity);
    const auto& map = registry.get<Map>(map_document.map);

    if (map.attached_tilesets.empty()) {
      prepare_vertical_alignment_center(2);
      push_centered_label(strings.misc.map_has_no_tilesets.c_str());

      ImGui::Spacing();

      if (push_centered_button(strings.action.add_tileset.c_str())) {
        dispatcher.enqueue<ShowNewTilesetDialogEvent>();
      }
    }
    else {
      push_tileset_tabs(registry, dispatcher);
    }
  }
}

auto is_tileset_dock_enabled(const Registry& registry) -> bool
{
  return sys::is_map_document_active(registry);
}

void on_mouse_wheel_event_in_tileset_dock(const Registry& registry,
                                          const Entity attached_tileset_entity,
                                          const cen::mouse_wheel_event& event,
                                          Dispatcher& dispatcher)
{
  const auto& viewport = registry.get<Viewport>(attached_tileset_entity);

  const Float2 scaling {4, 4};
  const Float2 precise {event.precise_x(), event.precise_y()};

  auto delta = precise * (viewport.tile_size / scaling);
  delta.x = -delta.x;

  dispatcher.enqueue<OffsetViewportEvent>(attached_tileset_entity, delta);
}

}  // namespace tactile::ui
