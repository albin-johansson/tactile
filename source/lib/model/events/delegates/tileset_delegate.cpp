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
#include "model/documents/command_system.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/entity_validation.hpp"
#include "model/maps/map_system.hpp"
#include "model/persistence/settings.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "ui/widget_state.hpp"

namespace tactile {

void on_show_new_tileset_dialog(Registry& registry, const ShowNewTilesetDialogEvent&)
{
  const auto& settings = registry.get<Settings>();

  auto& widgets = registry.get<ui::WidgetState>();
  widgets.new_tileset_dialog.map_entity = sys::get_active_map(registry);
  widgets.new_tileset_dialog.image_path.clear();
  widgets.new_tileset_dialog.image_path_preview_buffer.clear();
  widgets.new_tileset_dialog.tile_size = settings.get_preferred_tile_size();
  widgets.new_tileset_dialog.should_open = true;

  TACTILE_ASSERT(widgets.new_tileset_dialog.map_entity != kNullEntity);
}

void on_create_tileset(Registry& registry, const CreateTilesetEvent& event)
{
  TACTILE_ASSERT(sys::get_active_map(registry) == event.map);
  sys::try_execute<cmd::CreateTileset>(registry,
                                       event.map,
                                       event.tile_size,
                                       event.image_path);
}

void on_detach_tileset(Registry& registry, const DetachTilesetEvent& event)
{
  TACTILE_ASSERT(sys::get_active_map(registry) == event.map);
  sys::try_execute<cmd::RemoveTileset>(registry, event.map, event.attached_tileset);
}

void on_select_tileset(Registry& registry, const SelectTilesetEvent& event)
{
  const auto map_entity = sys::get_active_map(registry);
  if (map_entity != kNullEntity) {
    auto& map = registry.get<Map>(map_entity);
    sys::select_tileset(registry, map, event.attached_tileset);
  }
}

void on_set_tileset_selection(Registry& registry, const SetTilesetSelectionEvent& event)
{
  TACTILE_ASSERT(sys::is_attached_tileset_entity(registry, event.attached_tileset));

  auto& attached_tileset = registry.get<AttachedTileset>(event.attached_tileset);
  attached_tileset.selection = event.selection;
}

void on_rename_tileset(Registry& registry, const RenameTilesetEvent& event)
{
  TACTILE_ASSERT(sys::is_attached_tileset_entity(registry, event.attached_tileset));
  sys::try_execute<cmd::RenameTileset>(registry, event.attached_tileset, event.name);
}

void on_select_tileset_tile(Registry& registry, const SelectTilesetTileEvent& event)
{
  TACTILE_ASSERT(sys::is_tileset_document_entity(registry, event.tileset_document));
  const auto& tileset_document = registry.get<TilesetDocument>(event.tileset_document);

  auto& tileset = registry.get<Tileset>(tileset_document.tileset);
  tileset.selected_tile_index = event.tile_index;
}

void on_add_animation_frame(Registry& registry, const AddAnimationFrameEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(registry, event.tile));
  sys::try_execute<cmd::AddAnimationFrame>(registry,
                                           event.tile,
                                           event.frame_tile_index,
                                           event.frame_duration);
}

void on_set_tile_animation_frame_duration(Registry& registry,
                                          const SetAnimationFrameDurationEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(registry, event.tile));
  sys::try_execute<cmd::SetAnimationFrameDuration>(registry,
                                                   event.tile,
                                                   event.frame_index,
                                                   event.duration);
}

void on_enable_animation_frame_selection_mode(Registry& registry,
                                              const EnableAnimationFrameSelectionMode&)
{
  auto& tileset_viewport_state = registry.get<ui::TilesetViewportState>();
  tileset_viewport_state.animation_frame_selection_mode = true;
}

void on_remove_animation_frame(Registry& registry, const RemoveAnimationFrameEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(registry, event.tile));
  sys::try_execute<cmd::RemoveAnimationFrame>(registry, event.tile, event.frame_index);
}

void on_set_animation_frame_duration(Registry& registry,
                                     const SetAnimationFrameDurationEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(registry, event.tile));
  sys::try_execute<cmd::SetAnimationFrameDuration>(registry,
                                                   event.tile,
                                                   event.frame_index,
                                                   event.duration);
}

void on_move_animation_frame_forwards(Registry& registry,
                                      const MoveAnimationFrameForwardsEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(registry, event.tile));
  sys::try_execute<cmd::MoveAnimationFrameForwards>(registry,
                                                    event.tile,
                                                    event.frame_index);
}

void on_move_animation_frame_backwards(Registry& registry,
                                       const MoveAnimationFrameBackwardsEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(registry, event.tile));
  sys::try_execute<cmd::MoveAnimationFrameBackwards>(registry,
                                                     event.tile,
                                                     event.frame_index);
}

void on_rename_tile(Registry& registry, const RenameTileEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(registry, event.tile));
  sys::try_execute<cmd::RenameTile>(registry, event.tile, event.name);
}

void on_delete_tile_animation(Registry& registry, const DeleteTileAnimationEvent& event)
{
  TACTILE_ASSERT(sys::is_tile_entity(registry, event.tile));
  sys::try_execute<cmd::DeleteAnimation>(registry, event.tile);
}

}  // namespace tactile
