// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <nlohmann/json.hpp>

#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj/api.hpp"

namespace tactile {

class IMapView;

/**
 * Emits a single Tiled TMJ map JSON object.
 *
 * \param map The view of the map.
 *
 * \return
 * A map JSON object if successful; an error code otherwise.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/#map
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_tiled_tmj_map(const IMapView& map) -> nlohmann::json;

}  // namespace tactile
