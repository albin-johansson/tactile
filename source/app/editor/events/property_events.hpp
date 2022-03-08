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

#include <entt/entt.hpp>

#include "core/attribute.hpp"
#include "tactile.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct show_add_property_dialog_event final
{};

struct show_change_property_type_dialog_event final
{
  std::string name;
  attribute_type current_type{};
};

struct show_rename_property_dialog_event final
{
  std::string name;
};

struct add_property_event final
{
  std::string name;       ///< The chosen name.
  attribute_type type{};  ///< The chosen type.
};

struct remove_property_event final
{
  std::string name;  ///< Name of the property in the current context.
};

struct rename_property_event final
{
  std::string old_name;  ///< Current property name.
  std::string new_name;  ///< Requested new property name.
};

struct update_property_event final
{
  std::string name;       ///< Name of property to modify.
  attribute_value value;  ///< Updated value of the property.
};

struct change_property_type_event final
{
  std::string name;       ///< Name of property to modify.
  attribute_type type{};  ///< Requested new property type.
};

struct inspect_context_event final
{
  entt::entity entity{entt::null};  ///< Potentially null attribute context.
};

/// \} End of group events

}  // namespace tactile
