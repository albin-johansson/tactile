// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-json-format/api.hpp"
#include "tactile/tiled-json-format/common/json.hpp"

namespace tactile::tiled::tmj {

/**
 * \brief Emits a single TMJ layer JSON object.
 *
 * \param layer       the source layer.
 * \param tile_format the tile format used by tile layers.
 *
 * \return a JSON object.
 */
[[nodiscard]]
TACTILE_TMJ_API auto emit_layer(const ir::Layer& layer, const ir::TileFormat& tile_format)
    -> Result<JSON>;

/**
 * \brief Emits a JSON array of TMJ layers.
 *
 * \param map the source map.
 *
 * \return a JSON array.
 */
[[nodiscard]]
TACTILE_TMJ_API auto emit_layer_array(const ir::Map& map) -> Result<JSON>;

}  // namespace tactile::tiled::tmj
