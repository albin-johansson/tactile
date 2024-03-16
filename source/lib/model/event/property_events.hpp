// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/entity/entity.hpp>

#include "tactile/base/container/string.hpp"
#include "tactile/core/meta/attribute.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

struct ShowNewPropertyDialogEvent final {};

struct ShowChangePropertyTypeDialogEvent final {
  String name;
  AttributeType current_type {};
};

struct ShowRenamePropertyDialogEvent final {
  String current_name;
};

struct AddPropertyEvent final {
  UUID context_id {};     /// Target context.
  String name;            /// The property name.
  AttributeType type {};  /// The property type.
};

struct RemovePropertyEvent final {
  UUID context_id {};  /// Target context.
  String name;         ///< Name of the property in the current context.
};

struct RenamePropertyEvent final {
  UUID context_id {};  /// Target context.
  String old_name;     ///< Current property name.
  String new_name;     ///< Requested new property name.
};

struct UpdatePropertyEvent final {
  UUID context_id {};  /// Target context.
  String name;         ///< Name of property to modify.
  Attribute value;     ///< Updated value of the property.
};

struct ChangePropertyTypeEvent final {
  UUID context_id {};     /// Target context.
  String name;            ///< Name of property to modify.
  AttributeType type {};  ///< Requested new property type.
};

struct InspectContextEvent final {
  UUID context_id {};  /// Target context.
};

}  // namespace tactile
