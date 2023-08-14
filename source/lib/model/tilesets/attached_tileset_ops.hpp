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

#include "model/tilesets/tileset_components.hpp"
#include "tactile/core/common/prelude.hpp"
#include "tactile/core/functional/maybe.hpp"

namespace tactile::sys {

/// Indicates whether a global tile identifier is associated with the tileset.
[[nodiscard]] auto is_valid_tile(const AttachedTileset& attached_tileset, TileID tile_id)
    -> bool;

/// Converts a global tile identifier to a local tile index (as long as it's valid).
[[nodiscard]] auto to_tile_index(const AttachedTileset& attached_tileset, TileID tile_id)
    -> Maybe<TileIndex>;

/// Indicates whether a single tile is selected in the tileset.
[[nodiscard]] auto is_single_tile_selected(const AttachedTileset& attached_tileset)
    -> bool;

}  // namespace tactile::sys
