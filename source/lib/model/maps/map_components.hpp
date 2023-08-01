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

#include "core/ecs/entity.hpp"
#include "common/type/math.hpp"
#include "core/containers/vector.hpp"
#include "core/prelude.hpp"
#include "core/tiles/tile_extent.hpp"

namespace tactile {

/**
 * Primary component featured by map entities.
 *
 * \details Maps a constructed from multiple layers stacked on top of each other. An
 *          invisible group layer (the "root" layer) tracks the layers in each map. This
 *          root layer is always present, even in empty maps. However, this root layer is
 *          never included in save files, as it is merely an implementation detail to
 *          simplify the code.
 *
 * \details Each map has a collection of associated ("attached") tilesets that provide the
 *          tiles for use in tile layers. Tilesets may be shared with other maps in the
 *          case of external tilesets.
 *
 * \details Every map uses its own tile format specification, which can be used to control
 *          aspects such as whether tile layer data is compressed.
 */
struct Map final {
  TileExtent extent {5, 5};             ///< The size of the map in tiles.
  Int2 tile_size {32, 32};              ///< The logical size of all tiles.
  Entity root_layer {kNullEntity};      ///< The invisible root group layer.
  Entity active_layer {kNullEntity};    ///< The selected layer, if any.
  Entity active_tileset {kNullEntity};  ///< The active attached tileset, if any.
  Vector<Entity> attached_tilesets;     ///< The associated attached tilesets.
};

/// Map component storing available identifiers.
struct MapIdentifiers final {
  TileID next_tile_id {1};   ///< Next available global tile ID.
  int32 next_object_id {1};  ///< Next available object identifier.
  int32 next_layer_id {1};   ///< Next available layer identifier.
};

/// Map component storing integer suffixes used in generated layer names.
struct MapLayerSuffixes final {
  int32 tile_layer_suffix {1};    ///< Next tile layer suffix.
  int32 object_layer_suffix {1};  ///< Next object layer suffix.
  int32 group_layer_suffix {1};   ///< Next group layer suffix.
};

}  // namespace tactile
