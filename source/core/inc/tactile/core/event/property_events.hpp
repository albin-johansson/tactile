// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/meta/attribute.hpp"

namespace tactile {

/**
 * Event for opening the dialog for creating a new property.
 */
struct ShowNewPropertyDialogEvent final
{
  /** The target meta context. */
  EntityID context_entity;
};

/**
 * Event for creating and attaching a property to a meta context.
 */
struct CreatePropertyEvent final
{
  /** The target meta context. */
  EntityID context_entity;

  /** The new property name. */
  String name;

  /** The initial value of the property. */
  Attribute value;
};

}  // namespace tactile
