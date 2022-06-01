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

#include <vector>  // vector

#include <boost/uuid/uuid_hash.hpp>
#include <entt/fwd.hpp>

#include "core/common/associative.hpp"
#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/uuid.hpp"
#include "core/fwd.hpp"
#include "core/tile_pos.hpp"

namespace tactile::sys {

/**
 * \ingroup systems
 * \defgroup map-system Map System
 */

/// \addtogroup map-system
/// \{

/**
 * Appends a row of empty tiles to a map.
 *
 * \param registry a map registry.
 */
void add_row_to_map(entt::registry& registry);

/**
 * Appends a column of empty tiles to a map.
 *
 * \param registry a map registry.
 */
void add_column_to_map(entt::registry& registry);

/**
 * Removes the last row of tiles from a map.
 *
 * This function has no effect if there is only one row in the map.
 *
 * \param registry a map registry.
 */
void remove_row_from_map(entt::registry& registry);

/**
 * Removes the last column of tiles from a map.
 *
 * This function has no effect if there is only one column in the map.
 *
 * \param registry a map registry.
 */
void remove_column_from_map(entt::registry& registry);

/**
 * Sets the size of a map.
 *
 * \param registry a map registry.
 * \param nRows the new number of rows.
 * \param nCols the new number of columns.
 */
void resize_map(entt::registry& registry, usize nRows, usize nCols);

/// Maps previous invalid tile identifiers in a collection of layers.
using FixTilesInMapResult = HashMap<UUID, HashMap<TilePos, TileID>>;

/**
 * Validates all tiles in a map.
 *
 * This function iterates all tile layers and ensures that each stored tile identifier is
 * valid. All invalid tiles are set to be empty.
 *
 * \param registry a map registry.
 *
 * \return the found invalid tiles.
 */
auto fix_tiles_in_map(entt::registry& registry) -> FixTilesInMapResult;

/**
 * Indicates whether a position is within the bounds of a map.
 *
 * \param registry a map registry.
 * \param position the position that will be checked.
 *
 * \return `true` if the position is within the map; `false` otherwise.
 */
[[nodiscard]] auto is_position_in_map(const entt::registry& registry,
                                      const TilePos& position) -> bool;

/// \} End of group map-system

}  // namespace tactile::sys
