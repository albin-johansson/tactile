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
#include <spdlog/spdlog.h>

#include "core/documents/map_document.hpp"
#include "core/events/tileset_events.hpp"
#include "core/events/viewport_events.hpp"
#include "core/model.hpp"
#include "core/viewport.hpp"
#include "editor/ui/alignment.hpp"
#include "editor/ui/common/button.hpp"
#include "editor/ui/common/centered_text.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/tilesets/tileset_tabs.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile::ui {
namespace {

constexpr auto _window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

struct TilesetDockState final
{
  bool has_focus{};
  bool is_hovered{};
};

[[nodiscard]] auto _get_state() -> TilesetDockState&
{
  static TilesetDockState state;
  return state;
}

}  // namespace

void update_tileset_dock(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  auto& prefs = io::get_preferences();
  bool  visible = prefs.is_tileset_dock_visible();

  if (!visible) {
    return;
  }

  Window dock{"Tilesets", _window_flags, &visible};

  auto& state = _get_state();
  state.has_focus = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);
  state.is_hovered = ImGui::IsWindowHovered(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    const auto& document = model.require_active_map();
    const auto& map = document.get_map();

    if (map.get_tilesets().empty()) {
      prepare_vertical_alignment_center(2);
      centered_text("Current map has no tilesets!");

      ImGui::Spacing();

      if (centered_button(TAC_ICON_TILESET " Create tileset...")) {
        dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
      }
    }
    else {
      update_tileset_tabs(model, dispatcher);
    }
  }

  prefs.set_tileset_dock_visible(visible);
}

void tileset_dock_mouse_wheel_event_handler(const core::TilesetRef&       tilesetRef,
                                            const cen::mouse_wheel_event& event,
                                            entt::dispatcher&             dispatcher)
{
  constexpr Vector2f scaling{4, 4};

  const auto&    viewport = tilesetRef.viewport;
  const Vector2f precise{event.precise_x(), event.precise_y()};

  auto delta = precise * (viewport.get_cell_size() / scaling);
  delta.x = -delta.x;

  dispatcher.enqueue<OffsetTilesetViewportEvent>(tilesetRef.tileset->get_uuid(), delta);
}

auto is_tileset_dock_focused() -> bool
{
  return _get_state().has_focus;
}

auto is_tileset_dock_hovered() -> bool
{
  return _get_state().is_hovered;
}

}  // namespace tactile::ui
