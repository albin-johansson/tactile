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

#include <entt/fwd.hpp>

#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/tile_cache.hpp"
#include "core/components/layers.hpp"
#include "core/tile_pos.hpp"
#include "misc/assert.hpp"

namespace tactile::sys {

/**
 * \ingroup layer-system
 * \defgroup tile-layer-system Tile Layer System
 */

/// \addtogroup tile-layer-system
/// \{

/// \name Tile layer functions
/// \{

/**
 * \brief Returns the tile layer entity associated with a layer identifier.
 *
 * \param registry the document registry.
 * \param id the identifier associated with the desired layer.
 *
 * \return the layer entity.
 *
 * \throws TactileError if the identifier is invalid.
 */
[[nodiscard]] auto get_tile_layer_entity(const entt::registry& registry, const UUID& id)
    -> entt::entity;

/**
 * \brief Sets the value of a tile in a tile layer.
 *
 * \param layer the source tile layer.
 * \param position the position of the tile that will be modified.
 * \param tile the new tile value.
 *
 * \throws TactileError if the position is invalid.
 */
void set_tile(comp::TileLayer& layer, const TilePos& position, TileID tile);

/**
 * \brief Restores the values of tiles in a tile layer according to a tile cache.
 *
 * \pre The positions stored in the tile cache must be valid.
 *
 * \param layer the target tile layer.
 * \param tiles the source tile cache.
 */
void set_tiles(comp::TileLayer& layer, const TileCache& tiles);

/**
 * \brief Returns the tile at a specific position in a tile layer.
 *
 * \param layer the tile layer to query.
 * \param position the position to read.
 *
 * \return the tile at the position; the empty tile is returned for invalid positions.
 */
[[nodiscard]] auto get_tile(const comp::TileLayer& layer, const TilePos& position)
    -> TileID;

/**
 * \brief Visits each tile in a tile layer.
 *
 * \details The function object must provide `operator()(usize, usize, TileID)`.
 *
 * \param layer the tile layer to visit.
 * \param callable the function object invoked for each tile.
 */
void each_tile(const comp::TileLayer& layer, auto&& callable)
{
  TACTILE_ASSERT(!layer.matrix.empty());
  TACTILE_ASSERT(!layer.matrix.at(0).empty());

  const auto nRows = layer.matrix.size();
  const auto nCols = layer.matrix.at(0).size();

  for (usize row = 0; row < nRows; ++row) {
    for (usize col = 0; col < nCols; ++col) {
      const auto tile = layer.matrix[row][col];
      callable(row, col, tile);
    }
  }
}

/// \} End of tile layer functions

/// \} End of group tile-layer-system

}  // namespace tactile::sys
