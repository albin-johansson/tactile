/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/attribute.hpp"
#include "common/type/ecs.hpp"
#include "common/type/string.hpp"
#include "common/type/uuid.hpp"

namespace tactile {

struct ShowComponentEditorEvent final {};

struct ShowNewCompDialogEvent final {};

struct ShowRenameCompDialogEvent final {
  Entity definition {kNullEntity};  ///< A component definition entity.
};

struct ShowNewCompAttrDialogEvent final {
  Entity definition {kNullEntity};  ///< A component definition entity.
};

struct ShowRenameCompAttrDialogEvent final {
  Entity definition {kNullEntity};  ///< A component definition entity.
  String attr_name;                 ///< The target attribute.
};

/// Event for defining a new component.
struct DefineComponentEvent final {
  String name;  ///< Unique new component name.
};

/// Event for removing an existing component definition.
struct UndefComponentEvent final {
  Entity definition {kNullEntity};  ///< Target component to remove.
};

/// Event for renaming an existing component definition.
struct RenameComponentEvent final {
  Entity definition {kNullEntity};  ///< Target component to rename.
  String name;                      ///< The new component name.
};

/// Event for updating the default value of an attribute in a component definition.
struct UpdateComponentEvent final {
  Entity definition {kNullEntity};  ///< Target component definition.
  String attr_name;                 ///< Name of target attribute.
  Attribute value;                  ///< New default attribute value.
};

/// Event for adding an attribute to a component definition.
struct AddComponentAttrEvent final {
  Entity definition {kNullEntity};  ///< Target component definition.
  String attr_name;                 ///< Name of the new attribute.
};

/// Event for removing an attribute from a component definition.
struct RemoveComponentAttrEvent final {
  Entity definition {kNullEntity};  ///< Target component definition.
  String attr_name;                 ///< Name of the attribute to remove.
};

/// Event for changing the name of an attribute in a component definition.
struct RenameComponentAttrEvent final {
  Entity definition {kNullEntity};  ///< Target component definition.
  String current_name;              ///< Current attribute name.
  String updated_name;              ///< Requested new attribute name.
};

/// Event for duplicating an existing attribute in a component definition.
struct DuplicateComponentAttrEvent final {
  Entity definition {kNullEntity};  ///< Target component definition.
  String attr_name;                 ///< Name of attribute to duplicate.
};

/// Event for changing the type of an attribute in a component definition.
struct SetComponentAttrTypeEvent final {
  Entity definition {};   ///< Target component definition.
  String attr_name;       ///< Name of target attribute.
  AttributeType type {};  ///< New attribute type.
};

/// Event for adding a component to a context.
struct AttachComponentEvent final {
  Entity context {kNullEntity};     ///< Target context.
  Entity definition {kNullEntity};  ///< Definition of the component to attach.
};

/// Event for removing a component from a context.
struct DetachComponentEvent final {
  Entity context {kNullEntity};     ///< Target context.
  Entity definition {kNullEntity};  ///< Type of the component to detach.
};

/// Event for restoring the default attribute values in an attached component.
struct ResetAttachedComponentEvent final {
  Entity attached_component {kNullEntity};  ///< The target attached component.
};

/// Event for updating the value of an attribute in an attached component.
struct UpdateAttachedComponentEvent final {
  Entity attached_component {kNullEntity};  ///< The target attached component.
  String attr_name;                         ///< Name of target attribute.
  Attribute value;                          ///< New attribute value.
};

}  // namespace tactile