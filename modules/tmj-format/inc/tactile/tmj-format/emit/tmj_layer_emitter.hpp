// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/functional/result.hpp"
#include "tactile/core/io/ir.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/tmj-format/api.hpp"
#include "tactile/tmj-format/common/json.hpp"

namespace tactile::tmj {

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

}  // namespace tactile::tmj
