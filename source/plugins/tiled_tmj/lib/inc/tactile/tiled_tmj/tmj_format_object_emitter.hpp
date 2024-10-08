// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <nlohmann/json.hpp>

#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj/api.hpp"

namespace tactile {

class IObjectView;

/**
 * Emits a single Tiled TMJ object JSON node.
 *
 * \param object The view of the object.
 *
 * \return
 * An object JSON node.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/#object
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_tiled_tmj_object(const IObjectView& object) -> nlohmann::json;

}  // namespace tactile
