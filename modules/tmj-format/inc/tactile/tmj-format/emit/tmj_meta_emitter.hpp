// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/meta/attribute.hpp"
#include "tactile/foundation/meta/attribute_type.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tmj-format/api.hpp"
#include "tactile/tmj-format/common/json.hpp"

namespace tactile::tmj {

/**
 * \brief Returns the name for a given attribute type used in TMJ save files.
 *
 * \note The TMJ format doesn't support vector attribute types, so this function treats
 *       such types as strings.
 *
 * \param attribute_type the attribute type to query.
 *
 * \return a TMJ attribute type name.
 *
 * \todo Ability to control vector attribute type behavior?
 */
[[nodiscard]]
TACTILE_TMJ_API auto to_tmj_name(AttributeType attribute_type) -> StringView;

/**
 * \brief Converts an attribute to a TMJ-compatible JSON object.
 *
 * \param attribute the attribute to convert.
 *
 * \return a JSON value.
 */
[[nodiscard]]
TACTILE_TMJ_API auto to_tmj_value(const Attribute& attribute) -> JSON;

/**
 * \brief Emits a JSON array of TMJ properties.
 *
 * \param meta the source metadata.
 *
 * \return a JSON array.
 */
[[nodiscard]]
TACTILE_TMJ_API auto emit_property_array(const ir::Metadata& meta) -> JSON;

}  // namespace tactile::tmj
