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

#pragma once

#include "core/ecs/registry.hpp"
#include "model/events/tileset_events.hpp"

namespace tactile {

void on_show_new_tileset_dialog(Registry& registry,
                                const ShowNewTilesetDialogEvent& event);

void on_create_tileset(Registry& registry, const CreateTilesetEvent& event);

void on_detach_tileset(Registry& registry, const DetachTilesetEvent& event);

void on_select_tileset(Registry& registry, const SelectTilesetEvent& event);

void on_set_tileset_selection(Registry& registry, const SetTilesetSelectionEvent& event);

void on_rename_tileset(Registry& registry, const RenameTilesetEvent& event);

void on_select_tileset_tile(Registry& registry, const SelectTilesetTileEvent& event);

void on_add_animation_frame(Registry& registry, const AddAnimationFrameEvent& event);

void on_set_animation_frame_duration(Registry& registry,
                                     const SetAnimationFrameDurationEvent& event);

void on_enable_animation_frame_selection_mode(
    Registry& registry,
    const EnableAnimationFrameSelectionMode& event);

void on_remove_animation_frame(Registry& registry,
                               const RemoveAnimationFrameEvent& event);

void on_move_animation_frame_forwards(Registry& registry,
                                      const MoveAnimationFrameForwardsEvent& event);

void on_move_animation_frame_backwards(Registry& registry,
                                       const MoveAnimationFrameBackwardsEvent& event);

void on_rename_tile(Registry& registry, const RenameTileEvent& event);

void on_delete_tile_animation(Registry& registry, const DeleteTileAnimationEvent& event);

}  // namespace tactile
