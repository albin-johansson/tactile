/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include <string>  // string

#include "core/attribute.hpp"
#include "core/common/uuid.hpp"

namespace tactile {

struct OpenComponentEditorEvent final
{};

/// Event for defining a new component.
struct DefineComponentEvent final
{
  std::string name;  /// Unique new component name.
};

/// Event for removing an existing component definition.
struct UndefComponentEvent final
{
  UUID component_id {};  /// Target component to remove.
};

/// Event for renaming an existing component definition.
struct RenameComponentEvent final
{
  UUID        component_id {};  /// Target component to rename.
  std::string name;             /// The new component name.
};

/// Event for adding an attribute to a component definition.
struct AddComponentAttrEvent final
{
  UUID        component_id {};  /// Target component definition.
  std::string attr_name;        /// Name of the new attribute.
};

/// Event for removing an attribute from a component definition.
struct RemoveComponentAttrEvent final
{
  UUID        component_id {};  /// Target component definition.
  std::string attr_name;        /// Name of the attribute to remove.
};

/// Event for changing the name of an attribute in a component definition.
struct RenameComponentAttrEvent final
{
  UUID        component_id {};  /// Target component definition.
  std::string current_name;     /// Current attribute name.
  std::string updated_name;     /// Requested new attribute name.
};

/// Event for duplicating an existing attribute in a component definition.
struct DuplicateComponentAttrEvent final
{
  UUID        component_id {};  /// Target component definition.
  std::string attr_name;        /// Name of attribute to duplicate.
};

/// Event for changing the type of an attribute in a component definition.
struct SetComponentAttrTypeEvent final
{
  UUID          component_id {};  /// Target component definition.
  std::string   attr_name;        /// Name of target attribute.
  AttributeType type {};          /// New attribute type.
};

/// Event for updating the default value of an attribute in a component definition.
struct UpdateComponentEvent final
{
  UUID        component_id {};  /// Target component definition.
  std::string attr_name;        /// Name of target attribute.
  Attribute   value;            /// New default attribute value.
};

/// Event for adding a component to a context.
struct AttachComponentEvent final
{
  UUID context_id {};    /// Target context.
  UUID component_id {};  /// Definition of the component to attach.
};

/// Event for removing a component from a context.
struct DetachComponentEvent final
{
  UUID context_id {};    /// Target context.
  UUID component_id {};  /// Type of the component to detach.
};

/// Event for restoring the default attribute values in an attached component.
struct ResetAttachedComponentEvent final
{
  UUID context_id {};    /// Target context.
  UUID component_id {};  /// Type of the component to reset.
};

/// Event for updating the value of an attribute in an attached component.
struct UpdateAttachedComponentEvent final
{
  UUID        context_id {};    /// Target context.
  UUID        component_id {};  /// Type of the component to modify.
  std::string attr_name;        /// Name of target attribute.
  Attribute   value;            /// New attribute value.
};

}  // namespace tactile