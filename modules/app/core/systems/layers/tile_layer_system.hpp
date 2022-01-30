#pragma once

#include <entt/entt.hpp>

#include "assert.hpp"
#include "core/components/layer.hpp"
#include "core/tile_cache.hpp"
#include "core/tile_position.hpp"
#include "tactile_def.hpp"

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
[[nodiscard]] auto get_tile_layer_entity(const entt::registry& registry, layer_id id)
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
void set_tile(comp::tile_layer& layer, const tile_position& position, tile_id tile);

/**
 * \brief Restores the values of tiles in a tile layer according to a tile cache.
 *
 * \pre The positions stored in the tile cache must be valid.
 *
 * \param layer the target tile layer.
 * \param tiles the source tile cache.
 */
void set_tiles(comp::tile_layer& layer, const TileCache& tiles);

/**
 * \brief Returns the tile at a specific position in a tile layer.
 *
 * \param layer the tile layer to query.
 * \param position the position to read.
 *
 * \return the tile at the position; the empty tile is returned for invalid positions.
 */
[[nodiscard]] auto get_tile(const comp::tile_layer& layer, const tile_position& position)
    -> tile_id;

/**
 * \brief Visits each tile in a tile layer.
 *
 * \details The function object must provide `operator()(usize, usize, tile_id)`.
 *
 * \param layer the tile layer to visit.
 * \param callable the function object invoked for each tile.
 */
void each_tile(const comp::tile_layer& layer, auto&& callable)
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
