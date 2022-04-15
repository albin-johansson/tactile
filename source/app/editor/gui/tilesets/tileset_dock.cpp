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
#include "editor/gui/menus/map_menu.hpp"
#include "editor/model.hpp"
#include "io/persistence/preferences.hpp"
#include "tileset_tabs.hpp"

namespace tactile {
namespace {

constexpr auto _window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

}  // namespace

struct TilesetDock::Data final
{
  TilesetTabWidget tab_widget;
};

TilesetDock::TilesetDock()
    : ADockWidget{"Tilesets", _window_flags}
    , mData{std::make_unique<Data>()}
{
  set_focus_flags(ImGuiFocusedFlags_RootAndChildWindows);
  set_close_button_enabled(true);
}

TilesetDock::~TilesetDock() noexcept = default;

void TilesetDock::on_update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& registry = model.get_active_registry();

  if (!registry.view<comp::Tileset>().empty()) {
    mData->tab_widget.update(registry, dispatcher);
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

void TilesetDock::set_visible(const bool visible)
{
  get_preferences().set_tileset_dock_visible(visible);
}

auto TilesetDock::is_visible() const -> bool
{
  return get_preferences().is_tileset_dock_visible();
}

auto TilesetDock::get_tileset_view() const -> const TilesetView&
{
  return mData->tab_widget.get_tileset_view();
}

}  // namespace tactile
