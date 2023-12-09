// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tmj-format/api.hpp"
#include "tactile/tmj-format/common/json.hpp"

namespace tactile::tmj {

/**
 * \brief Converts a map object to a TMJ JSON object.
 *
 * \param object the object to convert.
 *
 * \return a JSON object.
 */
[[nodiscard]]
TACTILE_TMJ_API auto emit_object(const ir::Object& object) -> JSON;

/**
 * \brief Converts a group of map objects to an array of TMJ JSON objects.
 *
 * \param objects the objects to convert.
 *
 * \return a JSON array.
 */
[[nodiscard]]
TACTILE_TMJ_API auto emit_object_array(const Vector<ir::Object>& objects) -> JSON;

}  // namespace tactile::tmj
