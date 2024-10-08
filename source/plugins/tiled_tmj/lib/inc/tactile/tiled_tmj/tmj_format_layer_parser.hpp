// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>  // expected

#include <nlohmann/json.hpp>

#include "tactile/base/debug/error_code.hpp"
#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj/api.hpp"

namespace tactile {

class IRuntime;

/**
 * Attempts to parse object layer specified Tiled TMJ information.
 *
 * \param         layer_json The layer JSON node.
 * \param[in,out] layer      The object layer.
 *
 * \return
 * Nothing if successful; an error code otherwise.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/#layer
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto parse_tiled_tmj_object_layer(const nlohmann::json& layer_json,
                                                         ir::Layer& layer)
    -> std::expected<void, ErrorCode>;

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
    -> std::expected<ir::Layer, ErrorCode>;

}  // namespace tactile
