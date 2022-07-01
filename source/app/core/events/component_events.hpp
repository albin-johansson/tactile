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
#include "core/common/identifiers.hpp"
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
  UUID component_id{};  /// Target component to remove.
};

/// Event for renaming an existing component.
struct RenameComponentDefEvent final
{
  UUID        component_id{};  /// Target component to rename.
  std::string name;            /// The new component name.
};

struct CreateComponentAttrEvent final
{
  UUID        component_id{};
  std::string name;
};

struct RemoveComponentAttrEvent final
{
  UUID        component_id{};
  std::string name;
};

struct RenameComponentAttrEvent final
{
  UUID        component_id{};
  std::string previous;
  std::string updated;
};

struct DuplicateComponentAttrEvent final
{
  UUID        component_id{};
  std::string attribute;
};

struct SetComponentAttrTypeEvent final
{
  UUID          component_id{};
  std::string   attribute;
  AttributeType type{};
};

struct UpdateComponentDefAttrEvent final
{
  UUID        component_id{};
  std::string attribute;
  Attribute value;
};

struct AttachComponentEvent final
{
  UUID context_id{};    /// The target context.
  UUID component_id{};  /// A component definition ID.
};

struct DetachComponentEvent final
{
  ContextID context{};
  ComponentID component{};
};

struct ResetComponentValuesEvent final
{
  ContextID context{};
  ComponentID component{};
};

struct UpdateComponentEvent final
{
  ContextID context{};
  ComponentID component{};
  std::string attribute;
  Attribute value;
};

}  // namespace tactile