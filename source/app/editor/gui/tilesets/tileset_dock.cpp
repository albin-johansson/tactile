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

#include <entt/entt.hpp>
#include <imgui.h>

#include "core/components/tiles.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/gui/tilesets/tileset_tabs.hpp"
#include "editor/model.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile {
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
  auto& prefs = get_preferences();
  bool visible = prefs.is_tileset_dock_visible();

  if (!visible) {
    return;
  }

  scoped::Window dock{"Tilesets", _window_flags, &visible};

  auto& state = _get_state();
  state.has_focus = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);
  state.is_hovered = dock.current_window_contains_mouse();

  if (dock.is_open()) {
    const auto& registry = model.get_active_registry();
    if (registry.view<comp::Tileset>().empty()) {
      prepare_vertical_alignment_center(2);
      centered_text("Current map has no tilesets!");

      ImGui::Spacing();

      if (centered_button(TAC_ICON_TILESET " Create tileset...")) {
        dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
      }
    }
    else {
      update_tileset_tabs(registry, dispatcher);
    }
  }

  prefs.set_tileset_dock_visible(visible);
}

auto is_tileset_dock_focused() -> bool
{
  return _get_state().has_focus;
}

auto is_tileset_dock_hovered() -> bool
{
  return _get_state().is_hovered;
}

}  // namespace tactile
