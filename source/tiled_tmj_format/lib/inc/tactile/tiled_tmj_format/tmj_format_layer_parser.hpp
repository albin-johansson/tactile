// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <nlohmann/json.hpp>

#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/io/save/save_format_parse_result.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj_format/api.hpp"

namespace tactile {

class IRuntime;

/**
 * Attempts to parse a single Tiled TMJ layer.
 *
 * \param runtime    The associated runtime.
 * \param layer_json The layer JSON node.
 *
 * \return
 * The parsed layer if successful; an error code otherwise.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/#layer
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto parse_tiled_tmj_layer(const IRuntime& runtime,
                                                  const nlohmann::json& layer_json)
    -> SaveFormatParseResult<ir::Layer>;

}  // namespace tactile
