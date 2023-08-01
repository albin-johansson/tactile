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

#include "common/region.hpp"
#include "common/type/chrono.hpp"
#include "common/type/ecs.hpp"
#include "common/type/math.hpp"
#include "common/type/path.hpp"
#include "core/containers/string.hpp"
#include "core/prelude.hpp"

namespace tactile {

struct ShowNewTilesetDialogEvent final {};

struct InspectTilesetEvent final {
  Entity attached_tileset {kNullEntity};  ///< The attached tileset to inspect.
};

struct CreateTilesetEvent final {
  Entity map {kNullEntity};  ///< The map that the tileset will be attached to.
  Path image_path;           ///< Path to the tileset image.
  Int2 tile_size {};         ///< The logical size of tiles in the tileset.
};

struct DetachTilesetEvent final {
  Entity map {kNullEntity};               ///< The associated map.
  Entity attached_tileset {kNullEntity};  ///< The tileset that will be detached.
};

struct SelectTilesetEvent final {
  Entity attached_tileset {kNullEntity};
};

struct SetTilesetSelectionEvent final {
  Entity attached_tileset {kNullEntity};
  Region selection;
};

/// Event for changing the name a tileset.
struct RenameTilesetEvent final {
  Entity attached_tileset {kNullEntity};  ///< Target tileset.
  String name;                            ///< New tileset name.
};

/// Event for selecting a tile in an open tileset document.
struct SelectTilesetTileEvent final {
  Entity tileset_document {kNullEntity};
  TileIndex tile_index {};
};

/// Event for adding an animation frame to a tileset tile.
struct AddAnimationFrameEvent final {
  Entity tile {kNullEntity};      ///< The target tile.
  TileIndex frame_tile_index {};  ///< Tile index of shown during the frame.
  ms_t frame_duration {};         ///< Duration of the new frame.
};

/// Event for changing the duration of a frame in a tile animation.
struct SetAnimationFrameDurationEvent final {
  Entity tile {kNullEntity};  ///< Target tile.
  usize frame_index {};       ///< Index of frame that will be modified.
  ms_t duration {};           ///< New duration of the frame.
};

/// Event for enabling animation frame selection mode, where the next selected tile in the
/// tileset is added to the animation of the previously selected tile.
struct EnableAnimationFrameSelectionMode final {};

/// Event for removing an animation frame from the selected tileset tile.
struct RemoveAnimationFrameEvent final {
  Entity tile {kNullEntity};  ///< Target tile.
  usize frame_index {};       ///< Index of the frame that will be removed.
};

/// Event for moving a frame earlier in the animation sequence.
struct MoveAnimationFrameForwardsEvent final {
  Entity tile {kNullEntity};  ///< Target tile.
  usize frame_index {};       ///< The index of the target frame.
};

/// Event for moving a frame later in the animation sequence.
struct MoveAnimationFrameBackwardsEvent final {
  Entity tile {kNullEntity};  ///< Target tile.
  usize frame_index {};       ///< The index of the target frame.
};

/// Event for changing the name of a tile in a tileset.
struct RenameTileEvent final {
  Entity tile {kNullEntity};  ///< Target tile.
  String name;                ///< The new tile name.
};

/// Event for deleting the tile animation associated with the selected tileset tile.
struct DeleteTileAnimationEvent final {
  Entity tile {kNullEntity};  ///< Target tile.
};

}  // namespace tactile
