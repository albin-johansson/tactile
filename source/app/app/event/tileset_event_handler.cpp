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
#include <spdlog/spdlog.h>

#include "app/app_context.hpp"
#include "app/event/event_handlers.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "io/load_texture.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/tileset_events.hpp"
#include "model/model.hpp"
#include "ui/dock/tileset/dialogs/create_tileset_dialog.hpp"

namespace tactile {
namespace {

void on_inspect_tileset()
{
  auto& model = get_model();
  if (auto* document = model.active_tileset()) {
    document->get_contexts().select(document->view_tileset().get_uuid());
  }
}

void on_show_new_tileset_dialog()
{
  ui::open_create_tileset_dialog();
}

void on_load_tileset(const LoadTilesetEvent& event)
{
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
  auto& model = get_model();
  model.remove_tileset(event.tileset_id);
}

void on_select_tileset(const SelectTilesetEvent& event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    auto& tilesets = document->get_map().tileset_bundle();
    tilesets.select_tileset(event.tileset_id);
  }
}

void on_rename_tileset(const RenameTilesetEvent& event)
{
  auto& model = get_model();
  auto document = model.get_tileset(event.tileset_id);
  document->rename_tileset(event.name);
}

void on_set_tileset_selection(const SetTilesetSelectionEvent& event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    auto& tilesets = document->get_map().tileset_bundle();

    const auto tileset_id = tilesets.active_tileset_id().value();
    auto& tileset_ref = tilesets.get_ref(tileset_id);

    tileset_ref.set_selection(event.selection);
  }
}

}  // namespace

void install_tileset_event_handler()
{
  auto& dispatcher = get_dispatcher();

  dispatcher.sink<InspectTilesetEvent>().connect<&on_inspect_tileset>();

  // TODO rename event to ShowNewTilesetDialogEvent
  dispatcher.sink<ShowTilesetCreationDialogEvent>()
      .connect<&on_show_new_tileset_dialog>();

  dispatcher.sink<LoadTilesetEvent>().connect<&on_load_tileset>();
  dispatcher.sink<RemoveTilesetEvent>().connect<&on_remove_tileset>();
  dispatcher.sink<SelectTilesetEvent>().connect<&on_select_tileset>();
  dispatcher.sink<RenameTilesetEvent>().connect<&on_rename_tileset>();

  dispatcher.sink<SetTilesetSelectionEvent>().connect<&on_set_tileset_selection>();
}

}  // namespace tactile
