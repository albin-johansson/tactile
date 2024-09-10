// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>  // size_t

#include <nlohmann/json.hpp>

#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj_format/api.hpp"

namespace tactile {

class IMetaView;

/**
 * Emits a single Tiled TMJ property JSON object.
 *
 * \param meta           The view of the meta context.
 * \param property_index The index of the property to emit.
 *
 * \return
 * A property JSON object.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/#property
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_tiled_tmj_property(const IMetaView& meta,
                                                    std::size_t property_index)
    -> nlohmann::json;

/**
 * Emits the Tiled TMJ metadata for a given meta context.
 *
 * \param          meta      The view of the meta context.
 * \param[in, out] meta_json The target meta context JSON object.
 */
TACTILE_TMJ_FORMAT_API void emit_tiled_tmj_metadata(const IMetaView& meta,
                                                    nlohmann::json& meta_json);

}  // namespace tactile
