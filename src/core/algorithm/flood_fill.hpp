#pragma once

#include "layer.hpp"
#include "position.hpp"

namespace tactile::model {

/**
 * @brief Runs a flood fill in the supplied tile layer.
 *
 * @param layer the tile layer to run the flood fill in.
 * @param origin the starting position of the flood fill.
 * @param target the target tile type that will be replaced.
 * @param replacement the tile type that will be used as the replacement for
 * the target type.
 *
 * @since 0.1.0
 */
void flood_fill(layer& layer,
                const position& origin,
                tile_id target,
                tile_id replacement);

}  // namespace tactile::core