// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>  // expected

#include <nlohmann/json.hpp>

#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/debug/error_code.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj/api.hpp"

namespace tactile {

/**
 * Attempts to parse a single Tiled TMJ layer object.
 *
 * \param object_json The object JSON node.
 *
 * \return
 * The parsed object if successful; an error code otherwise.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/#object
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto parse_tiled_tmj_object(const nlohmann::json& object_json)
    -> std::expected<ir::Object, ErrorCode>;

}  // namespace tactile
