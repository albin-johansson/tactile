// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>  // expected

#include <nlohmann/json.hpp>

#include "tactile/base/debug/error_code.hpp"
#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj/api.hpp"

namespace tactile {

class IRuntime;

/**
 * Attempts to parse a single Tiled TMJ map.
 *
 * \param runtime  The associated runtime.
 * \param map_json The map JSON node.
 * \param options  The configured read options.
 *
 * \return
 * The parsed map if successful; an error code otherwise.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/#map
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto parse_tiled_tmj_map(const IRuntime& runtime,
                                                const nlohmann::json& map_json,
                                                const SaveFormatReadOptions& options)
    -> std::expected<ir::Map, ErrorCode>;

}  // namespace tactile
