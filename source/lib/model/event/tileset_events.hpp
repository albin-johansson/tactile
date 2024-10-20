// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/region.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/util/chrono.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

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
  TileIndex tile_index {};   ///< Tile index of the tile that features the animation.
  usize frame_index {};      ///< Index of frame that will be modified.
  Milliseconds duration {};  ///< New duration of the frame.
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
