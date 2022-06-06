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

#include <functional>  // function
#include <vector>      // vector

#include <entt/fwd.hpp>

#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/tile_cache.hpp"
#include "core/fwd.hpp"
#include "misc/assert.hpp"

namespace tactile::sys {

/**
 * Floods a tile layer with a tile at the specified location.
 *
 * \param layer the target tile layer.
 * \param origin the origin position of the flood fill.
 * \param replacement the tile identifier used to replace the origin identifier.
 * \param[out] affected the positions of the tiles affected by the flood fill.
 */
void flood(comp::TileLayer& layer,
           const TilePos& origin,
           TileID replacement,
           std::vector<TilePos>& affected);

/**
 * Sets the value of a tile.
 *
 * \param layer the source tile layer.
 * \param pos the position of the tile that will be modified.
 * \param id the new tile value.
 *
 * \throws TactileError if the position is invalid.
 */
void set_tile(comp::TileLayer& layer, const TilePos& pos, TileID id);

/**
 * Restores the values of tiles according to a tile cache.
 *
 * \pre The positions stored in the tile cache must be valid.
 *
 * \param layer the target tile layer.
 * \param tiles the source tile cache.
 */
void set_tiles(comp::TileLayer& layer, const TileCache& tiles);

/**
 * Returns the tile at a specific position in a tile layer.
 *
 * \param layer the tile layer to query.
 * \param pos the position to read.
 *
 * \return the tile at the position;
 *         an empty tile for invalid positions.
 */
[[nodiscard]] auto get_tile(const comp::TileLayer& layer, const TilePos& pos) -> TileID;

/**
 * Visits each tile in a tile layer.
 *
 * \param layer the tile layer to visit.
 * \param callable the function object invoked for each tile.
 */
void each_tile(const comp::TileLayer& layer,
               const std::function<void(usize, usize, TileID)>& callable);

/// Indicates whether a position is valid in a tile layer.
[[nodiscard]] auto is_valid_position(const comp::TileLayer& layer, const TilePos& pos)
    -> bool;

}  // namespace tactile::sys
