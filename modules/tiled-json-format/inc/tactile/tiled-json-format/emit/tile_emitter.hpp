// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-json-format/api.hpp"
#include "tactile/tiled-json-format/common.hpp"

namespace tactile::tiled::tmj {

/**
 * Converts a tile definition to a TMJ JSON object.
 *
 * \param tile The tile definition.
 *
 * \return
 *    A JSON object.
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_tile_definition(const ir::Tile& tile) -> JSON;

/**
 * Converts a collection of tile definitions to a TMJ JSON array.
 *
 * \param tiles The tile definitions.
 *
 * \return
 *    A JSON array.
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_tile_definition_array(const Vector<ir::Tile>& tiles)
    -> JSON;

}  // namespace tactile::tiled::tmj
