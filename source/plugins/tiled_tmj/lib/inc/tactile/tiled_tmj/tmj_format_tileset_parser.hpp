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

/**
 * Attempts to parse a single Tiled TMJ tileset.
 *
 * \param tileset_json The tileset JSON node.
 * \param options      The read options.
 *
 * \return
 * A tileset if successful; an error code otherwise.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/#tileset
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto parse_tiled_tmj_tileset(const nlohmann::json& tileset_json,
                                                    const SaveFormatReadOptions& options)
    -> std::expected<ir::TilesetRef, ErrorCode>;

}  // namespace tactile
