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

#pragma once

#include "common/numeric.hpp"
#include "common/type/chrono.hpp"
#include "common/type/math.hpp"
#include "common/type/path.hpp"
#include "common/type/string.hpp"
#include "common/type/uuid.hpp"
#include "core/region.hpp"

namespace tactile {

struct ShowTilesetCreationDialogEvent final {};

struct InspectTilesetEvent final {};

struct LoadTilesetEvent final {
  Path path;
  Int2 tile_size {};
};

struct RemoveTilesetEvent final {
  UUID tileset_id {};
};

struct SelectTilesetEvent final {
  UUID tileset_id {};
};

struct SetTilesetSelectionEvent final {
  Region selection;
};

/// Event for changing the name a tileset.
struct RenameTilesetEvent final {
  UUID tileset_id {};  /// Target tileset.
  String name;         /// New tileset name.
};

/// Event for selecting a tile in an open tileset document.
struct SelectTilesetTileEvent final {
  TileIndex tile_index {};
};

/// Event for changing the duration of a frame in a tile animation.
struct SetTileAnimationFrameDurationEvent final {
  TileIndex tile_index {};  ///< Tile index of the tile that features the animation.
  usize frame_index {};     ///< Index of frame that will be modified.
  ms_t duration {};         ///< New duration of the frame.
};

/// Event for enabling animation frame selection mode, where the next selected tile in the
/// tileset is added to the animation of the previously selected tile.
struct EnableAnimationFrameSelectionMode final {};

/// Event for adding an animation frame to the selected tileset tile.
struct AddTileAnimationFrameEvent final {
  TileIndex tile_index {};  ///< Tile that will be added as a frame to the animation.
};

/// Event for removing an animation frame from the selected tileset tile.
struct RemoveTileAnimationFrameEvent final {
  usize frame_index {};  ///< Index of frame that will be removed from the animation.
};

/// Event for moving a frame earlier in the animation sequence.
struct MoveAnimationFrameForwardsEvent final {
  usize frame_index {};
};

/// Event for moving a frame later in the animation sequence.
struct MoveAnimationFrameBackwardsEvent final {
  usize frame_index {};
};

/// Event for changing the name of a tile in a tileset.
struct RenameTileEvent final {
  TileIndex tile_index {};
  String name;
};

/// Event for deleting the tile animation associated with the selected tileset tile.
struct DeleteTileAnimationEvent final {};

}  // namespace tactile
