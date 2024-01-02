// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-json-format/api.hpp"
#include "tactile/tiled-json-format/common.hpp"

namespace tactile::tiled::tmj {

/**
 * Converts a map object to a TMJ JSON object.
 *
 * \param object The object to convert.
 *
 * \return
 *    A JSON object.
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_object(const ir::Object& object) -> JSON;

/**
 * Converts a group of map objects to an array of TMJ JSON objects.
 *
 * \param objects The objects to convert.
 *
 * \return
 *    A JSON array.
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_object_array(const Vector<ir::Object>& objects) -> JSON;

}  // namespace tactile::tiled::tmj
