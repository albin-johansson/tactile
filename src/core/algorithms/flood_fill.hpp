#pragma once

#include <vector>  // vector

#include "aliases/tile_id.hpp"
#include "core/map/layers/tile_layer.hpp"
#include "core/map/map_position.hpp"

namespace Tactile {

/**
 * \brief Runs a flood fill in the supplied tile layer.
 *
 * \param layer the tile layer to run the flood fill in.
 * \param origin the starting position of the flood fill.
 * \param replacement the tile type that will be used as the replacement.
 * \param[out] affected the vector that the affected positions will be added to.
 */
void FloodFill(TileLayer& layer,
               const MapPosition& origin,
               tile_id replacement,
               std::vector<MapPosition>& affected);

}  // namespace Tactile