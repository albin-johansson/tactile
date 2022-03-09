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

#include <imgui.h>

#include "core/components/tiles.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/menus/map_menu.hpp"
#include "editor/gui/scoped.hpp"
#include "io/persistence/preferences.hpp"
#include "tileset_tabs.hpp"

namespace tactile {
namespace {

constexpr auto gWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

}  // namespace

void TilesetDock::Update(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  ResetState();

  auto& prefs = get_preferences();
  bool visible = prefs.is_tileset_dock_visible();

  if (!visible) {
    return;
  }

  scoped::Window window{"Tilesets", gWindowFlags, &visible};
  if (window.is_open()) {
    mHasFocus = ImGui::IsWindowFocused();
    mWindowContainsMouse = ImGui::IsWindowHovered(ImGuiFocusedFlags_RootAndChildWindows);

    if (!registry.view<comp::Tileset>().empty()) {
      mTabWidget.Update(registry, dispatcher);
    }
    else {
      prepare_vertical_alignment_center(2);
      centered_text("Current map has no tilesets!");
      ImGui::Spacing();
      if (centered_button(TAC_ICON_TILESET " Create tileset...")) {
        dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
      }
    }
  }

  prefs.set_tileset_dock_visible(visible);
}

auto TilesetDock::GetTilesetView() const -> const TilesetView&
{
  return mTabWidget.GetTilesetView();
}

void TilesetDock::ResetState()
{
  mHasFocus = false;
  mWindowContainsMouse = false;
}

}  // namespace tactile
