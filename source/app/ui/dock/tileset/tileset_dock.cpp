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

#include "tileset_dock.hpp"

#include <centurion/mouse_events.hpp>
#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/tile/tileset_bundle.hpp"
#include "core/viewport.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "model/event/tileset_events.hpp"
#include "model/event/viewport_events.hpp"
#include "model/model.hpp"
#include "tileset_tabs.hpp"
#include "ui/style/alignment.hpp"
#include "ui/widget/buttons.hpp"
#include "ui/widget/labels.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

constinit bool dock_has_focus = false;
constinit bool dock_has_hover = false;

}  // namespace

void update_tileset_dock(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  auto& prefs = io::get_preferences();

  if (!prefs.show_tileset_dock) {
    return;
  }

  const auto& lang = get_current_language();

  Window dock {lang.window.tileset_dock.c_str(),
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
               &prefs.show_tileset_dock};

  // We intentionally do not use the window is_hovered function here
  dock_has_focus = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);
  dock_has_hover = ImGui::IsWindowHovered(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    const auto& document = model.require_active_map();
    const auto& map = document.get_map();

    if (map.tileset_bundle().empty()) {
      prepare_vertical_alignment_center(2);
      centered_label(lang.misc.map_has_no_tilesets.c_str());

      ImGui::Spacing();

      if (centered_button(lang.action.add_tileset.c_str())) {
        dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
      }
    }
    else {
      update_tileset_tabs(model, dispatcher);
    }
  }
}

void tileset_dock_mouse_wheel_event_handler(const TilesetRef& tileset_ref,
                                            const cen::mouse_wheel_event& event,
                                            entt::dispatcher& dispatcher)
{
  constexpr Float2 scaling {4, 4};

  const auto& viewport = tileset_ref.get_viewport();
  const Float2 precise {event.precise_x(), event.precise_y()};

  auto delta = precise * (viewport.tile_size() / scaling);
  delta.x = -delta.x;

  dispatcher.enqueue<OffsetTilesetViewportEvent>(tileset_ref.view_tileset().get_uuid(),
                                                 delta);
}

auto is_tileset_dock_focused() -> bool
{
  return dock_has_focus;
}

auto is_tileset_dock_hovered() -> bool
{
  return dock_has_hover;
}

}  // namespace tactile::ui
