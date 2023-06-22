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

#include "tileset_delegate.hpp"

#include "cmd/tile/add_animation_frame.hpp"
#include "cmd/tile/delete_animation.hpp"
#include "cmd/tile/move_animation_frame_backwards.hpp"
#include "cmd/tile/move_animation_frame_forwards.hpp"
#include "cmd/tile/remove_animation_frame.hpp"
#include "cmd/tile/rename_tile.hpp"
#include "cmd/tile/set_animation_frame_duration.hpp"
#include "cmd/tileset/create_tileset.hpp"
#include "cmd/tileset/remove_tileset.hpp"
#include "cmd/tileset/rename_tileset.hpp"
#include "common/debug/assert.hpp"
#include "model/components/document.hpp"
#include "model/documents/command_system.hpp"
#include "model/documents/document_system.hpp"
#include "model/settings.hpp"
#include "model/systems/map_system.hpp"
#include "model/systems/validation_system.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "ui/widget_state.hpp"

namespace tactile {

void on_show_new_tileset_dialog(Model& model, const ShowNewTilesetDialogEvent&)
{
  const auto& settings = model.get<Settings>();

  auto& widgets = model.get<ui::WidgetState>();
  widgets.new_tileset_dialog.map_entity = sys::get_active_map(model);
  widgets.new_tileset_dialog.image_path.clear();
  widgets.new_tileset_dialog.image_path_preview_buffer.clear();
  widgets.new_tileset_dialog.tile_size = settings.get_preferred_tile_size();
  widgets.new_tileset_dialog.should_open = true;

  TACTILE_ASSERT(widgets.new_tileset_dialog.map_entity != kNullEntity);
}

void on_create_tileset(Model& model, const CreateTilesetEvent& event)
{
  TACTILE_ASSERT(sys::get_active_map(model) == event.map);
  sys::try_execute<cmd::CreateTileset>(model,
                                       event.map,
                                       event.tile_size,
                                       event.image_path);
}

void on_detach_tileset(Model& model, const DetachTilesetEvent& event)
{
  TACTILE_ASSERT(sys::get_active_map(model) == event.map);
  sys::try_execute<cmd::RemoveTileset>(model, event.map, event.attached_tileset);
}

void on_select_tileset(Model& model, const SelectTilesetEvent& event)
{
  const auto map_entity = sys::get_active_map(model);
  if (map_entity != kNullEntity) {
    auto& map = model.get<Map>(map_entity);
    sys::select_tileset(model, map, event.attached_tileset);
  }
}

void on_set_tileset_selection(Model& model, const SetTilesetSelectionEvent& event)
{
  TACTILE_ASSERT(sys::is_attached_tileset_entity(model, event.attached_tileset));

  auto& attached_tileset = model.get<AttachedTileset>(event.attached_tileset);
  attached_tileset.selection = event.selection;
}

void on_rename_tileset(Model& model, const RenameTilesetEvent& event)
{
  TACTILE_ASSERT(sys::is_attached_tileset_entity(model, event.attached_tileset));
  sys::try_execute<cmd::RenameTileset>(model, event.attached_tileset, event.name);
}

void on_select_tileset_tile(Model& model, const SelectTilesetTileEvent& event)
{
  TACTILE_ASSERT(sys::is_tileset_document_entity(model, event.tileset_document));
  const auto& tileset_document = model.get<TilesetDocument>(event.tileset_document);

  auto& tileset = model.get<Tileset>(tileset_document.tileset);
  tileset.selected_tile_index = event.tile_index;
}

void on_add_animation_frame(Model& model, const AddAnimationFrameEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(model, event.tile));
  sys::try_execute<cmd::AddAnimationFrame>(model,
                                           event.tile,
                                           event.frame_tile_index,
                                           event.frame_duration);
}

void on_set_tile_animation_frame_duration(Model& model,
                                          const SetAnimationFrameDurationEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(model, event.tile));
  sys::try_execute<cmd::SetAnimationFrameDuration>(model,
                                                   event.tile,
                                                   event.frame_index,
                                                   event.duration);
}

void on_enable_animation_frame_selection_mode(Model& model,
                                              const EnableAnimationFrameSelectionMode&)
{
  auto& tileset_viewport_state = model.get<ui::TilesetViewportState>();
  tileset_viewport_state.animation_frame_selection_mode = true;
}

void on_remove_animation_frame(Model& model, const RemoveAnimationFrameEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(model, event.tile));
  sys::try_execute<cmd::RemoveAnimationFrame>(model, event.tile, event.frame_index);
}

void on_set_animation_frame_duration(Model& model,
                                     const SetAnimationFrameDurationEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(model, event.tile));
  sys::try_execute<cmd::SetAnimationFrameDuration>(model,
                                                   event.tile,
                                                   event.frame_index,
                                                   event.duration);
}

void on_move_animation_frame_forwards(Model& model,
                                      const MoveAnimationFrameForwardsEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(model, event.tile));
  sys::try_execute<cmd::MoveAnimationFrameForwards>(model, event.tile, event.frame_index);
}

void on_move_animation_frame_backwards(Model& model,
                                       const MoveAnimationFrameBackwardsEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(model, event.tile));
  sys::try_execute<cmd::MoveAnimationFrameBackwards>(model,
                                                     event.tile,
                                                     event.frame_index);
}

void on_rename_tile(Model& model, const RenameTileEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(model, event.tile));
  sys::try_execute<cmd::RenameTile>(model, event.tile, event.name);
}

void on_delete_tile_animation(Model& model, const DeleteTileAnimationEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(model, event.tile));
  sys::try_execute<cmd::DeleteAnimation>(model, event.tile);
}

}  // namespace tactile
