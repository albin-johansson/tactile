// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <system_error>  // error_code

#include <nlohmann/json.hpp>

#include "tactile/base/io/byte_stream.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj/api.hpp"

namespace tactile {

class IRuntime;
class ILayerView;

/**
 * Emits a single Tiled TMJ layer JSON object.
 *
 * \param runtime          The associated runtime.
 * \param layer            The view of the layer.
 * \param tile_byte_stream The byte stream used to store encoded tile bytes. This is only used
 *                         for performance reasons when emitting tile layers.
 *
 * \return
 * A layer JSON object if successful; an error code otherwise.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/#layer
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_tiled_tmj_layer(const IRuntime& runtime,
                                                 const ILayerView& layer,
                                                 ByteStream& tile_byte_stream)
    -> std::expected<nlohmann::json, std::error_code>;

}  // namespace tactile
