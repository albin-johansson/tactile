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

#include "common/numeric.hpp"
#include "common/type/ecs.hpp"
#include "common/type/math.hpp"
#include "common/type/vec.hpp"
#include "core/tile_extent.hpp"

namespace tactile {

/// Component representing a tile map.
///
/// Maps a built from multiple layers stacked on top of each other. An invisible group
/// layer (the "root" layer) manages the layers in every map. This root layer is always
/// present, even in empty maps. However, this root layer is never included in save files,
/// it is merely an implementation detail to simplify the map code.
///
/// Each map has a collection of associated tilesets, providing the tiles to be used in
/// tile layers. Tilesets may be shared with other maps in the case of external tilesets.
/// See the Tileset class for more information.
///
/// Every map uses its own tile format specification, which can be used to control aspects
/// such as whether tile layer data is compressed. By default, maps do not use any
/// compression along with plain text encoding.
struct Map final {
  TileExtent extent {5, 5};                   ///< Total size of the map, in tiles.
  Int2 tile_size {32, 32};                    ///< Logical size of all tiles.
  GroupLayerEntity root_layer {kNullEntity};  ///< Invisible root layer.
  LayerEntity active_layer {kNullEntity};     ///< The selected layer.
  AttachedTilesetEntity active_tileset {kNullEntity};  ///< The active attached tileset).
  Vec<AttachedTilesetEntity> attached_tilesets;        ///< The associated tilesets.
  TileID next_tile_id {1};                             ///< Next available global tile ID.
  int32 next_object_id {1};       ///< Next available object identifier.
  int32 next_layer_id {1};        ///< Next available layer identifier.
  int32 tile_layer_suffix {1};    ///< Next tile layer suffix (for naming purposes).
  int32 object_layer_suffix {1};  ///< Next object layer suffix (for naming purposes).
  int32 group_layer_suffix {1};   ///< Next group layer suffix (for naming purposes).
};

}  // namespace tactile
