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

#include <entt/entity/entity.hpp>

#include "core/attribute.hpp"
#include "core/common/uuid.hpp"

namespace tactile {

struct ShowAddPropertyDialogEvent final
{};

struct ShowChangePropertyTypeDialogEvent final
{
  std::string   name;
  AttributeType current_type{};
};

struct ShowRenamePropertyDialogEvent final
{
  std::string name;
};

struct AddPropertyEvent final
{
  UUID          context_id{};  /// Target context.
  std::string   name;          /// The property name.
  AttributeType type{};        /// The property type.
};

struct RemovePropertyEvent final
{
  UUID        context_id{};  /// Target context.
  std::string name;          ///< Name of the property in the current context.
};

struct RenamePropertyEvent final
{
  UUID        context_id{};  /// Target context.
  std::string old_name;      ///< Current property name.
  std::string new_name;      ///< Requested new property name.
};

struct UpdatePropertyEvent final
{
  UUID        context_id{};  /// Target context.
  std::string name;          ///< Name of property to modify.
  Attribute   value;         ///< Updated value of the property.
};

struct ChangePropertyTypeEvent final
{
  UUID          context_id{};  /// Target context.
  std::string   name;          ///< Name of property to modify.
  AttributeType type{};        ///< Requested new property type.
};

struct InspectContextEvent final
{
  UUID context_id{};  /// Target context.
};

}  // namespace tactile
