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
#include "core/components/component.hpp"
#include "tactile.hpp"

namespace tactile {

struct open_component_editor_event final
{};

struct create_component_def_event final
{
  std::string name;
};

struct rename_component_def_event final
{
  component_id id{};
  std::string name;
};

struct remove_component_def_event final
{
  component_id id{};
};

struct create_component_attr_event final
{
  component_id id{};
  std::string name;
};

struct remove_component_attr_event final
{
  component_id id{};
  std::string name;
};

struct rename_component_attr_event final
{
  component_id id{};
  std::string previous;
  std::string updated;
};

struct duplicate_component_attr_event final
{
  component_id id{};
  std::string attribute;
};

struct set_component_attr_type_event final
{
  component_id id{};
  std::string attribute;
  AttributeType type{};
};

struct update_component_def_attr_event final
{
  component_id id{};
  std::string attribute;
  Attribute value;
};

struct add_component_event final
{
  context_id context{};
  component_id component{};
};

struct remove_component_event final
{
  context_id context{};
  component_id component{};
};

struct reset_component_values_event final
{
  context_id context{};
  component_id component{};
};

struct update_component_event final
{
  context_id context{};
  component_id component{};
  std::string attribute;
  Attribute value;
};

}  // namespace tactile