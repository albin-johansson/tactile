// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>  // string

#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

/**
 * Event for opening the dialog for creating a new property.
 */
struct ShowNewPropertyDialogEvent final
{
  /** The target meta context. */
  EntityID context_entity;
};

/**
 * Event for opening the dialog for renaming a property.
 */
struct ShowRenamePropertyDialogEvent final
{
  /** The associated meta context. */
  EntityID context_entity;

  /** The target property. */
  std::string name;
};

/**
 * Event for creating and attaching a property to a meta context.
 */
struct CreatePropertyEvent final
{
  /** The target meta context. */
  EntityID context_entity;

  /** The new property name. */
  std::string name;

  /** The initial value of the property. */
  Attribute value;
};

/**
 * Event for updating the value of a property in a meta context.
 */
struct UpdatePropertyEvent final
{
  /** The target meta context. */
  EntityID context_entity;

  /** The target property name. */
  std::string name;

  /** The new value of the property. */
  Attribute value;
};

/**
 * Event for removing a property from a meta context.
 */
struct RemovePropertyEvent final
{
  /** The target meta context. */
  EntityID context_entity;

  /** The target property name. */
  std::string name;
};

/**
 * Event for renaming a property in a meta context.
 */
struct RenamePropertyEvent final
{
  /** The target meta context. */
  EntityID context_entity;

  /** The old property name. */
  std::string old_name;

  /** The new property name. */
  std::string new_name;
};

}  // namespace tactile::core
