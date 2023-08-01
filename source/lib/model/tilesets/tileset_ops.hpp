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

#include "common/tile_pos.hpp"
#include "common/type/ecs.hpp"
#include "core/prelude.hpp"
#include "model/tilesets/tileset_components.hpp"

namespace tactile::sys {

/// Returns the amount of tiles in the tileset.
[[nodiscard]] auto tile_count(const Tileset& tileset) -> int32;

/// Returns the index of the tile at a specific position.
[[nodiscard]] auto tile_index_at(const Tileset& tileset, TilePos pos) -> TileIndex;

/// Indicates whether a position refers to a tile in the tileset.
[[nodiscard]] auto is_valid_tile(const Tileset& tileset, TilePos pos) -> bool;

/// Returns the currently selected tile, if there is one.
[[nodiscard]] auto get_active_tile(const Tileset& tileset) -> Entity;

}  // namespace tactile::sys
