#pragma once

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "assert.hpp"
#include "core/components/layer.hpp"
#include "core/tile_cache.hpp"
#include "core/tile_position.hpp"

namespace tactile::sys {

/**
 * \ingroup systems
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
[[nodiscard]] auto get_tile_layer_entity(const entt::registry& registry, LayerID id)
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
void set_tile(TileLayer& layer, const tile_position& position, TileID tile);

/**
 * \brief Restores the values of tiles in a tile layer according to a tile cache.
 *
 * \pre The positions stored in the tile cache must be valid.
 *
 * \param layer the target tile layer.
 * \param tiles the source tile cache.
 */
void set_tiles(TileLayer& layer, const TileCache& tiles);

/**
 * \brief Returns the tile at a specific position in a tile layer.
 *
 * \param layer the tile layer to query.
 * \param position the position to read.
 *
 * \return the tile at the position; the empty tile is returned for invalid positions.
 */
[[nodiscard]] auto get_tile(const TileLayer& layer, const tile_position& position)
    -> TileID;

/**
 * \brief Visits each tile in a tile layer.
 *
 * \tparam T the function object type, must provide `operator()(usize, usize, TileID)`.
 *
 * \param layer the tile layer to visit.
 * \param callable the function object invoked for each tile.
 */
template <typename T>
void each_tile(const TileLayer& layer, T&& callable)
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
