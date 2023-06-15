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

namespace tactile {

struct ShowNewPropertyDialogEvent final {
  Entity context {kNullEntity};  ///< The target context.
};

struct ShowRenamePropertyDialogEvent final {
  Entity context {kNullEntity};  ///< The target context.
  String property_name;          ///< The current property name.
};

struct ShowSetPropertyTypeDialogEvent final {
  Entity context {kNullEntity};  ///< The target context.
  String property_name;          ///< The name of the target property.
};

struct InspectContextEvent final {
  Entity context {kNullEntity};  ///< Target context.
};

struct CreatePropertyEvent final {
  Entity context {kNullEntity};  ///< Target context.
  String name;                   ///< The property name.
  AttributeType type {};         ///< The property type.
};

struct RemovePropertyEvent final {
  Entity context {kNullEntity};  ///< Target context.
  String name;                   ///< Name of the property in the current context.
};

struct RenamePropertyEvent final {
  Entity context {kNullEntity};  ///< Target context.
  String old_name;               ///< Current property name.
  String new_name;               ///< Requested new property name.
};

struct UpdatePropertyEvent final {
  Entity context {kNullEntity};  ///< Target context.
  String name;                   ///< Name of property to modify.
  Attribute value;               ///< Updated value of the property.
};

struct SetPropertyTypeEvent final {
  Entity context {kNullEntity};  ///< Target context.
  String name;                   ///< Name of property to modify.
  AttributeType type {};         ///< Requested new property type.
};

}  // namespace tactile
