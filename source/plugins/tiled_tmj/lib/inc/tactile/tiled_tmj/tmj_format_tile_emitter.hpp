// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <nlohmann/json.hpp>

#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj/api.hpp"

namespace tactile {

class ITileView;

/**
 * Emits a single Tiled TMJ tile JSON node.
 *
 * \param tile The view of the tile.
 *
 * \return
 * A tile definition JSON node.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/#tile-definition
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_tiled_tmj_tile(const ITileView& tile) -> nlohmann::json;

}  // namespace tactile
