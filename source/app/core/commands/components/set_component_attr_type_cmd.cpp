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

#include "set_component_attr_type_cmd.hpp"

#include <utility>  // move

namespace tactile {

SetComponentAttrTypeCmd::SetComponentAttrTypeCmd(RegistryRef registry,
                                                 const ComponentID& id,
                                                 std::string attribute,
                                                 const AttributeType type)
    : ACommand{"Set Component Attribute Type"}
    , mRegistry{registry}
    , mComponentId{id}
    , mAttributeName{std::move(attribute)}
    , mNewType{type}
{}

void SetComponentAttrTypeCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::restore_component_attribute_type(registry, mSnapshot.value());
  mSnapshot.reset();
}

void SetComponentAttrTypeCmd::redo()
{
  auto& registry = mRegistry.get();

  mSnapshot =
      sys::set_component_attribute_type(registry, mComponentId, mAttributeName, mNewType);
}

}  // namespace tactile