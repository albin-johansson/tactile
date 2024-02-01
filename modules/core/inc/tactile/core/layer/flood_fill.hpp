// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/misc/tile_pos.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

class TileLayer;

/**
 * Applies a flood fill algorithm to a layer.
 *
 * \details
 *    This is currently implemented using a "combined-scan-and-fill" algorithm, see
 *    the section about "span filling" in the Wikipedia article about flood fill
 *    algorithms for more details.
 *
 * \param      layer              The layer to flood.
 * \param      start_pos          The starting position.
 * \param      new_id             The replacement tile identifier.
 * \param[out] affected_positions A vector to which all modified positions are recorded.
 *
 * \see https://en.wikipedia.org/wiki/Flood_fill
 */
TACTILE_CORE_API void flood_fill(TileLayer& layer,
                                 const TilePos& start_pos,
                                 TileID new_id,
                                 Vector<TilePos>* affected_positions = nullptr);

}  // namespace tactile::core
