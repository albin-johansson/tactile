// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <system_error>  // error_code

#include <nlohmann/json.hpp>

#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/io/save/save_format_parse_error.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj/api.hpp"

namespace tactile {

/**
 * Attempts to parse a single Tiled TMJ property object.
 *
 * \param property_json The property JSON object.
 *
 * \return
 * The parsed property if successful; an error code otherwise.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/#property
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto parse_tiled_tmj_property(const nlohmann::json& property_json)
    -> std::expected<ir::NamedAttribute, SaveFormatParseError>;

/**
 * Attempts to parse the metadata associated with a generic Tiled TMJ JSON object.
 *
 * \details
 * It's safe to pass a JSON object without any associated metadata to this function.
 *
 * \note
 * This function will try to parse and fill in the name field of the metadata structure, but it
 * won't fail if there is no \c "name" attribute.
 *
 * \param root_json The JSON object that might feature metadata.
 *
 * \return
 * The parsed metadata if successful; an error code otherwise.
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto parse_tiled_tmj_metadata(const nlohmann::json& root_json)
    -> std::expected<ir::Metadata, SaveFormatParseError>;

}  // namespace tactile
