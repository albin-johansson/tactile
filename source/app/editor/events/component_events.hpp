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
#include "core/components/attributes.hpp"
#include "tactile.hpp"

namespace tactile {

struct OpenComponentEditorEvent final
{};

struct CreateComponentDefEvent final
{
  std::string name;
};

struct RenameComponentDefEvent final
{
  component_id id{};
  std::string name;
};

struct RemoveComponentDefEvent final
{
  component_id id{};
};

struct CreateComponentAttrEvent final
{
  component_id id{};
  std::string name;
};

struct RemoveComponentAttrEvent final
{
  component_id id{};
  std::string name;
};

struct RenameComponentAttrEvent final
{
  component_id id{};
  std::string previous;
  std::string updated;
};

struct DuplicateComponentAttrEvent final
{
  component_id id{};
  std::string attribute;
};

struct SetComponentAttrTypeEvent final
{
  component_id id{};
  std::string attribute;
  AttributeType type{};
};

struct UpdateComponentDefAttrEvent final
{
  component_id id{};
  std::string attribute;
  Attribute value;
};

struct AddComponentEvent final
{
  context_id context{};
  component_id component{};
};

struct RemoveComponentEvent final
{
  context_id context{};
  component_id component{};
};

struct ResetComponentValuesEvent final
{
  context_id context{};
  component_id component{};
};

struct UpdateComponentEvent final
{
  context_id context{};
  component_id component{};
  std::string attribute;
  Attribute value;
};

}  // namespace tactile