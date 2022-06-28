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


namespace tactile {

struct OpenComponentEditorEvent final
{};

struct CreateComponentDefEvent final
{
  std::string name;
};

struct RenameComponentDefEvent final
{
  ComponentID id{};
  std::string name;
};

struct RemoveComponentDefEvent final
{
  ComponentID id{};
};

struct CreateComponentAttrEvent final
{
  ComponentID id{};
  std::string name;
};

struct RemoveComponentAttrEvent final
{
  ComponentID id{};
  std::string name;
};

struct RenameComponentAttrEvent final
{
  ComponentID id{};
  std::string previous;
  std::string updated;
};

struct DuplicateComponentAttrEvent final
{
  ComponentID id{};
  std::string attribute;
};

struct SetComponentAttrTypeEvent final
{
  ComponentID id{};
  std::string attribute;
  AttributeType type{};
};

struct UpdateComponentDefAttrEvent final
{
  ComponentID id{};
  std::string attribute;
  Attribute value;
};

struct AddComponentEvent final
{
  ContextID context{};
  ComponentID component{};
};

struct RemoveComponentEvent final
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