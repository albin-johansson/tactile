#pragma once

#include <vector>  // vector

#include "position.hpp"
#include "tile_layer.hpp"

namespace tactile::core {

/**
 * \brief Runs a flood fill in the supplied tile layer.
 *
 * \param layer the tile layer to run the flood fill in.
 * \param origin the starting position of the flood fill.
 * \param replacement the tile type that will be used as the replacement.
 * \param[out] affected the vector that the affected positions will be added to.
 *
 * \since 0.1.0
 */
void FloodFill(TileLayer& layer,
               const position& origin,
               tile_id replacement,
               std::vector<position>& affected);

}  // namespace tactile::core