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

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <fmt/chrono.h>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/util/fmt.hpp"
#include "core/tile/tile.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "handlers/event_handlers.hpp"
#include "io/load_texture.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/tileset_events.hpp"
#include "model/model.hpp"
#include "runtime/app_context.hpp"
#include "ui/dock/tileset/dialogs/create_tileset_dialog.hpp"
#include "ui/viewport/tileset_viewport.hpp"

namespace tactile {
namespace {

void on_inspect_tileset(const InspectTilesetEvent&)
{
  spdlog::trace("InspectTilesetEvent");

  if (auto* tileset_document = get_model().active_tileset_document()) {
    const auto& tileset = tileset_document->get_tileset();
    tileset_document->get_contexts().select_context(tileset.get_uuid());
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

  if (auto texture = load_texture(event.path)) {
    get_model().add_tileset(TilesetInfo {
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

  get_model().remove_tileset(event.tileset_id);
}

void on_select_tileset(const SelectTilesetEvent& event)
{
  spdlog::trace("SelectTilesetEvent(tileset_id: {})", event.tileset_id);

  if (auto* map_document = get_model().active_map_document()) {
    auto& tileset_bundle = map_document->get_map().get_tileset_bundle();
    tileset_bundle.select_tileset(event.tileset_id);
  }
}

void on_rename_tileset(const RenameTilesetEvent& event)
{
  spdlog::trace("RenameTilesetEvent(tileset_id: {}, name: {})",
                event.tileset_id,
                event.name);

  auto tileset_document = get_model().get_tileset_document_ptr(event.tileset_id);
  tileset_document->rename_tileset(event.name);
}

void on_set_tileset_selection(const SetTilesetSelectionEvent& event)
{
  spdlog::trace("SetTilesetSelectionEvent(region: (begin: {}, end: {}))",
                event.selection.begin,
                event.selection.end);

  if (auto* map_document = get_model().active_map_document()) {
    auto& tileset_bundle = map_document->get_map().get_tileset_bundle();

    const auto tileset_id = tileset_bundle.get_active_tileset_id().value();
    auto& tileset_ref = tileset_bundle.get_tileset_ref(tileset_id);

    tileset_ref.set_selection(event.selection);
  }
}

void on_select_tileset_tile(const SelectTilesetTileEvent& event)
{
  spdlog::trace("SelectTilesetTileEvent(tile_index: {})", event.tile_index);

  if (auto* tileset_document = get_model().active_tileset_document()) {
    auto& tileset = tileset_document->get_tileset();
    tileset.select_tile(event.tile_index);

    const auto& tile_id = tileset[event.tile_index].get_uuid();
    tileset_document->get_contexts().select_context(tile_id);
  }
}

void on_set_tile_animation_frame_duration(const SetTileAnimationFrameDurationEvent& event)
{
  spdlog::trace(
      "SetTileAnimationFrameDurationEvent(tile_index: {}, frame_index: {}, duration: {})",
      event.tile_index,
      event.frame_index,
      event.duration);

  if (auto* tileset_document = get_model().active_tileset_document()) {
    tileset_document->set_animation_frame_duration(event.tile_index,
                                                   event.frame_index,
                                                   event.duration);
  }
}

void on_enable_animation_frame_selection_mode(const EnableAnimationFrameSelectionMode&)
{
  spdlog::trace("EnableAnimationFrameSelectionMode");
  ui::enable_tile_animation_frame_selection_mode();
}

void on_add_tile_animation_frame(const AddTileAnimationFrameEvent& event)
{
  spdlog::trace("AddTileAnimationFrameEvent(tile_index: {})", event.tile_index);

  if (auto* tileset_document = get_model().active_tileset_document()) {
    const auto& tileset = tileset_document->get_tileset();
    if (const auto selected_tile_index = tileset.get_selected_tile()) {
      tileset_document->add_animation_frame(*selected_tile_index,
                                            event.tile_index,
                                            Milliseconds {1'000});
    }
  }
}

void on_remove_tile_animation_frame(const RemoveTileAnimationFrameEvent& event)
{
  spdlog::trace("RemoveTileAnimationFrameEvent(frame_index: {})", event.frame_index);

  if (auto* tileset_document = get_model().active_tileset_document()) {
    const auto& tileset = tileset_document->get_tileset();
    if (const auto selected_tile_index = tileset.get_selected_tile()) {
      tileset_document->remove_animation_frame(*selected_tile_index, event.frame_index);
    }
  }
}

void on_move_animation_frame_forwards(const MoveAnimationFrameForwardsEvent& event)
{
  spdlog::trace("MoveAnimationFrameForwardsEvent(frame_index: {})", event.frame_index);

  if (auto* tileset_document = get_model().active_tileset_document()) {
    const auto& tileset = tileset_document->get_tileset();
    if (const auto selected_tile_index = tileset.get_selected_tile()) {
      tileset_document->move_animation_frame_forwards(*selected_tile_index,
                                                      event.frame_index);
    }
  }
}

void on_move_animation_frame_backwards(const MoveAnimationFrameBackwardsEvent& event)
{
  spdlog::trace("MoveAnimationFrameBackwardsEvent(frame_index: {})", event.frame_index);

  if (auto* tileset_document = get_model().active_tileset_document()) {
    const auto& tileset = tileset_document->get_tileset();
    if (const auto selected_tile_index = tileset.get_selected_tile()) {
      tileset_document->move_animation_frame_backwards(*selected_tile_index,
                                                       event.frame_index);
    }
  }
}

void on_delete_tile_animation(const DeleteTileAnimationEvent&)
{
  spdlog::trace("DeleteTileAnimationEvent");

  if (auto* tileset_document = get_model().active_tileset_document()) {
    const auto& tileset = tileset_document->get_tileset();
    if (const auto selected_tile_index = tileset.get_selected_tile()) {
      tileset_document->delete_animation(*selected_tile_index);
    }
  }
}

void on_rename_tile(const RenameTileEvent& event)
{
  spdlog::trace("RenameTileEvent(tile_index: {}, name: {})",
                event.tile_index,
                event.name);

  if (auto* tileset_document = get_model().active_tileset_document()) {
    tileset_document->rename_tile(event.tile_index, event.name);
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
  dispatcher.sink<EnableAnimationFrameSelectionMode>().connect<&on_enable_animation_frame_selection_mode>();
  dispatcher.sink<AddTileAnimationFrameEvent>().connect<&on_add_tile_animation_frame>();
  dispatcher.sink<RemoveTileAnimationFrameEvent>().connect<&on_remove_tile_animation_frame>();
  dispatcher.sink<MoveAnimationFrameForwardsEvent>().connect<&on_move_animation_frame_forwards>();
  dispatcher.sink<MoveAnimationFrameBackwardsEvent>().connect<&on_move_animation_frame_backwards>();
  dispatcher.sink<DeleteTileAnimationEvent>().connect<&on_delete_tile_animation>();
  dispatcher.sink<RenameTileEvent>().connect<&on_rename_tile>();
  // clang-format on
}

}  // namespace tactile
