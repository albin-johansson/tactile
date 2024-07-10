// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/string_map.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/**
 * Represents an attribute bundle.
 *
 * \note
 * Attribute bundles are known as "components" in the UI, we don't use that
 * terminology in code to avoid confusion with ECS components.
 */
using AttributeBundle = StringMap<Attribute>;

/**
 * A component that provides common metadata.
 */
struct CMeta final
{
  /** The name associated with the context. */
  String name;

  /** The attached properties. */
  StringMap<Attribute> properties;

  /** The attached attribute bundles. */
  HashMap<UUID, AttributeBundle> components;
};

}  // namespace tactile
