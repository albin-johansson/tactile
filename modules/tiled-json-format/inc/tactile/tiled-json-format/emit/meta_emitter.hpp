// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/meta/attribute.hpp"
#include "tactile/foundation/meta/attribute_type.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-json-format/api.hpp"
#include "tactile/tiled-json-format/common.hpp"

namespace tactile::tiled::tmj {

/**
 * Returns the name for a given attribute type used in TMJ save files.
 *
 * \note
 *    The TMJ format doesn't support vector attribute types, so this
 *    function treats such types as strings.
 *
 * \param attribute_type The attribute type to query.
 *
 * \return
 *    A TMJ attribute type name.
 *
 * \todo Ability to control vector attribute type behavior?
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto to_tmj_name(AttributeType attribute_type) -> StringView;

/**
 * Converts an attribute to a TMJ-compatible JSON object.
 *
 * \param attribute The attribute to convert.
 *
 * \return
 *    A JSON value.
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto to_tmj_value(const Attribute& attribute) -> JSON;

/**
 * Emits a JSON array of TMJ properties.
 *
 * \param meta The source metadata.
 *
 * \return
 *    A JSON array.
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_property_array(const ir::Metadata& meta) -> JSON;

}  // namespace tactile::tiled::tmj
