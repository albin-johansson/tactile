// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/string.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

struct OpenComponentEditorEvent final {};

/// Event for defining a new component.
struct DefineComponentEvent final {
  String name;  /// Unique new component name.
};

/// Event for removing an existing component definition.
struct UndefComponentEvent final {
  UUID component_id {};  /// Target component to remove.
};

/// Event for renaming an existing component definition.
struct RenameComponentEvent final {
  UUID component_id {};  /// Target component to rename.
  String name;           /// The new component name.
};

/// Event for adding an attribute to a component definition.
struct AddComponentAttrEvent final {
  UUID component_id {};  /// Target component definition.
  String attr_name;      /// Name of the new attribute.
};

/// Event for removing an attribute from a component definition.
struct RemoveComponentAttrEvent final {
  UUID component_id {};  /// Target component definition.
  String attr_name;      /// Name of the attribute to remove.
};

/// Event for changing the name of an attribute in a component definition.
struct RenameComponentAttrEvent final {
  UUID component_id {};  /// Target component definition.
  String current_name;   /// Current attribute name.
  String updated_name;   /// Requested new attribute name.
};

/// Event for duplicating an existing attribute in a component definition.
struct DuplicateComponentAttrEvent final {
  UUID component_id {};  /// Target component definition.
  String attr_name;      /// Name of attribute to duplicate.
};

/// Event for changing the type of an attribute in a component definition.
struct SetComponentAttrTypeEvent final {
  UUID component_id {};   /// Target component definition.
  String attr_name;       /// Name of target attribute.
  AttributeType type {};  /// New attribute type.
};

/// Event for updating the default value of an attribute in a component definition.
struct UpdateComponentEvent final {
  UUID component_id {};  /// Target component definition.
  String attr_name;      /// Name of target attribute.
  Attribute value;       /// New default attribute value.
};

/// Event for adding a component to a context.
struct AttachComponentEvent final {
  UUID context_id {};    /// Target context.
  UUID component_id {};  /// Definition of the component to attach.
};

/// Event for removing a component from a context.
struct DetachComponentEvent final {
  UUID context_id {};    /// Target context.
  UUID component_id {};  /// Type of the component to detach.
};

/// Event for restoring the default attribute values in an attached component.
struct ResetAttachedComponentEvent final {
  UUID context_id {};    /// Target context.
  UUID component_id {};  /// Type of the component to reset.
};

/// Event for updating the value of an attribute in an attached component.
struct UpdateAttachedComponentEvent final {
  UUID context_id {};    /// Target context.
  UUID component_id {};  /// Type of the component to modify.
  String attr_name;      /// Name of target attribute.
  Attribute value;       /// New attribute value.
};

}  // namespace tactile
