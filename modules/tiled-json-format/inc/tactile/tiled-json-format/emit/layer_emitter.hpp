// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-json-format/api.hpp"
#include "tactile/tiled-json-format/common.hpp"

namespace tactile::tiled::tmj {

/**
 * Emits a single TMJ layer JSON object.
 *
 * \param layer       The source layer.
 * \param tile_format The tile format used by tile layers.
 *
 * \return
 *    A JSON object.
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_layer(const ir::Layer& layer,
                                       const ir::TileFormat& tile_format) -> Result<JSON>;

/**
 * Emits a JSON array of TMJ layers.
 *
 * \param map The source map.
 *
 * \return
 *    A JSON array.
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_layer_array(const ir::Map& map) -> Result<JSON>;

}  // namespace tactile::tiled::tmj
