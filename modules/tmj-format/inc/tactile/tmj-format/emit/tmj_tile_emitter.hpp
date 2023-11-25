// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/io/ir.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/tmj-format/api.hpp"
#include "tactile/tmj-format/common/json.hpp"

namespace tactile::tmj {

/**
 * \brief Converts a tile definition to a TMJ JSON object.
 *
 * \param tile the tile definition.
 *
 * \return a JSON object.
 */
[[nodiscard]]
TACTILE_TMJ_API auto emit_tile_definition(const ir::Tile& tile) -> JSON;

/**
 * \brief Converts a collection of tile definitions to a TMJ JSON array.
 *
 * \param tiles the tile definitions.
 *
 * \return a JSON array.
 */
[[nodiscard]]
TACTILE_TMJ_API auto emit_tile_definition_array(const Vector<ir::Tile>& tiles) -> JSON;

}  // namespace tactile::tmj