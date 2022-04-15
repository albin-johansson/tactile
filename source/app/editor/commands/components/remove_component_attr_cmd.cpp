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

#include "remove_component_attr_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

RemoveComponentAttrCmd::RemoveComponentAttrCmd(RegistryRef registry,
                                               const ComponentID componentId,
                                               std::string attribute)
    : ACommand{"Remove Component Attribute"}
    , mRegistry{registry}
    , mComponentId{componentId}
    , mAttributeName{std::move(attribute)}
{}

void RemoveComponentAttrCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::make_component_attribute(registry,
                                mComponentId,
                                mAttributeName,
                                mPreviousDefault.value());
  mPreviousDefault.reset();
}

void RemoveComponentAttrCmd::redo()
{
  auto& registry = mRegistry.get();
  mPreviousDefault =
      sys::get_component_attribute_value(registry, mComponentId, mAttributeName);
  sys::remove_component_attribute(registry, mComponentId, mAttributeName);
}

}  // namespace tactile