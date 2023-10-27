// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/functional/result.hpp"
#include "tactile/core/io/ir.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/tmj-format/api.hpp"
#include "tactile/tmj-format/common/json.hpp"

namespace tactile::tmj {

/**
 * \brief Parses a single map object.
 *
 * \details The JSON could look something like the following.
 *          \code
 *          {
 *            "id": 42,
 *            "name": "foo",
 *            "type": "tag",
 *            "x": 0,
 *            "y": 0,
 *            "width": 100,
 *            "height": 100,
 *            "visible": true,
 *            "properties": [],
 *          }
 *          \endcode
 *
 * \param object_json the map object JSON.
 *
 * \return the parsed map object; or an error code if something went wrong.
 */
[[nodiscard]]
TACTILE_TMJ_API auto parse_object(const JSON& object_json) -> Result<ir::Object>;

/**
 * \brief Parses a single map layer.
 *
 * \details The JSON could look something like the following.
 *          \code
 *          {
 *            "name": "foo",
 *            "type": "tilelayer",
 *            "data": [1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3],
 *            "width": 4,
 *            "height": 3,
 *            "opacity": 1,
 *            "visible": true,
 *            "x": 0,
 *            "y": 0,
 *            "properties": []
 *          }
 *          \endcode
 *
 * \param layer_json the layer JSON.
 * \param map        the associated map.
 *
 * \return the parsed layer; or an error code if something went wrong.
 */
[[nodiscard]]
TACTILE_TMJ_API auto parse_layer(const JSON& layer_json, ir::Map& map)
    -> Result<ir::Layer>;

/**
 * \brief Parses all layers in a TMJ map.
 *
 * \param map_json the map JSON object.
 * \param map      the associated map.
 *
 * \return nothing; or an error code if something went wrong.
 */
[[nodiscard]]
TACTILE_TMJ_API auto parse_layers(const JSON& map_json, ir::Map& map) -> Result<void>;

}  // namespace tactile::tmj
