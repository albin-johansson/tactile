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

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <fmt/chrono.h>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "app/app_context.hpp"
#include "app/event/event_handlers.hpp"
#include "core/tile/tile.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "core/util/fmt.hpp"
#include "io/load_texture.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/tileset_events.hpp"
#include "model/model.hpp"
#include "ui/dock/tileset/dialogs/create_tileset_dialog.hpp"

namespace tactile {
namespace {

void on_inspect_tileset(const InspectTilesetEvent&)
{
  spdlog::trace("InspectTilesetEvent");

  auto& model = get_model();
  if (auto* document = model.active_tileset()) {
    document->get_contexts().select(document->view_tileset().get_uuid());
  }
}

void on_show_new_tileset_dialog(const ShowTilesetCreationDialogEvent&)
{
  spdlog::trace("ShowTilesetCreationDialogEvent");
  ui::open_create_tileset_dialog();
}

void on_load_tileset(const LoadTilesetEvent& event)
{
  spdlog::trace("LoadTilesetEvent(path: {}, tile_size: {})", event.path, event.tile_size);

  if (auto texture = io::load_texture(event.path)) {
    auto& model = get_model();
    model.add_tileset(TilesetInfo {
        .texture = std::move(texture),
        .tile_size = event.tile_size,
    });
  }
  else {
    spdlog::error("Failed to load tileset texture!");
  }
}

void on_remove_tileset(const RemoveTilesetEvent& event)
{
  spdlog::trace("RemoveTilesetEvent(tileset_id: {})", event.tileset_id);

  auto& model = get_model();
  model.remove_tileset(event.tileset_id);
}

void on_select_tileset(const SelectTilesetEvent& event)
{
  spdlog::trace("SelectTilesetEvent(tileset_id: {})", event.tileset_id);

  auto& model = get_model();
  if (auto* document = model.active_map()) {
    auto& tilesets = document->get_map().tileset_bundle();
    tilesets.select_tileset(event.tileset_id);
  }
}

void on_rename_tileset(const RenameTilesetEvent& event)
{
  spdlog::trace("RenameTilesetEvent(tileset_id: {}, name: {})",
                event.tileset_id,
                event.name);

  auto& model = get_model();
  auto document = model.get_tileset(event.tileset_id);
  document->rename_tileset(event.name);
}

void on_set_tileset_selection(const SetTilesetSelectionEvent& event)
{
  spdlog::trace("SetTilesetSelectionEvent(region: (begin: {}, end: {}))",
                event.selection.begin,
                event.selection.end);

  auto& model = get_model();
  if (auto* document = model.active_map()) {
    auto& tilesets = document->get_map().tileset_bundle();

    const auto tileset_id = tilesets.active_tileset_id().value();
    auto& tileset_ref = tilesets.get_ref(tileset_id);

    tileset_ref.set_selection(event.selection);
  }
}

void on_select_tileset_tile(const SelectTilesetTileEvent& event)
{
  spdlog::trace("SelectTilesetTileEvent(tile_index: {})", event.tile_index);

  auto& model = get_model();
  if (auto* document = model.active_tileset()) {
    auto& tileset = document->view_tileset();
    tileset.select_tile(event.tile_index);
  }
}

void on_set_tile_animation_frame_duration(const SetTileAnimationFrameDurationEvent& event)
{
  spdlog::trace(
      "SetTileAnimationFrameDurationEvent(tile_index: {}, frame_index: {}, duration: {})",
      event.tile_index,
      event.frame_index,
      event.duration);

  auto& model = get_model();
  if (auto* document = model.active_tileset()) {
    auto& tileset = document->view_tileset();

    // TODO command
    auto& tile = tileset[event.tile_index];
    auto& animation = tile.get_animation();
    animation[event.frame_index].duration = event.duration;
  }
}

}  // namespace

void install_tileset_event_handler()
{
  auto& dispatcher = get_dispatcher();

  // clang-format off
  // TODO rename event to ShowNewTilesetDialogEvent
  dispatcher.sink<ShowTilesetCreationDialogEvent>().connect<&on_show_new_tileset_dialog>();

  dispatcher.sink<InspectTilesetEvent>().connect<&on_inspect_tileset>();
  dispatcher.sink<LoadTilesetEvent>().connect<&on_load_tileset>();
  dispatcher.sink<RemoveTilesetEvent>().connect<&on_remove_tileset>();
  dispatcher.sink<SelectTilesetEvent>().connect<&on_select_tileset>();
  dispatcher.sink<RenameTilesetEvent>().connect<&on_rename_tileset>();
  dispatcher.sink<SetTilesetSelectionEvent>().connect<&on_set_tileset_selection>();
  dispatcher.sink<SelectTilesetTileEvent>().connect<&on_select_tileset_tile>();
  dispatcher.sink<SetTileAnimationFrameDurationEvent>().connect<&on_set_tile_animation_frame_duration>();
  // clang-format on
}

}  // namespace tactile
