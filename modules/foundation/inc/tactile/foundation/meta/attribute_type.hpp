// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/** \brief The supported property value types. */
enum class AttributeType : int8 {
  kStr,
  kInt,
  kInt2,
  kInt3,
  kInt4,
  kFloat,
  kFloat2,
  kFloat3,
  kFloat4,
  kBool,
  kPath,
  kColor,
  kObject,
};

/**
 * \brief Attempts to convert a type name into the corresponding attribute type.
 *
 * \details The supported type names are `string`, `int`, `int2`, `int3`, `int4`, `float`,
 *          `float2`, `float3`, `float4`, `bool`, `file`, `path`, `color`, and `object`.
 *          Note that `file` and `path` are synonyms for the path attribute type.
 *
 * \param name the type name.
 * \return the parsed attribute type.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto parse_attribute_type(StringView name) -> Maybe<AttributeType>;

}  // namespace tactile
