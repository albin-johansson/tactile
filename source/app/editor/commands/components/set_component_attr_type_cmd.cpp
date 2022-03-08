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

#include "core/systems/component_system.hpp"

namespace tactile {

set_component_attr_type_cmd::set_component_attr_type_cmd(registry_ref registry,
                                                         const component_id id,
                                                         std::string attribute,
                                                         const attribute_type type)
    : ACommand{"Set Component Attribute Type"}
    , mRegistry{registry}
    , mComponentId{id}
    , mAttributeName{std::move(attribute)}
    , mNewType{type}
{}

void set_component_attr_type_cmd::undo()
{
  auto& registry = mRegistry.get();

  const auto previous = mPreviousValue.value();

  sys::set_component_attribute_type(registry,
                                    mComponentId,
                                    mAttributeName,
                                    previous.type());
  sys::set_component_attribute_value(registry, mComponentId, mAttributeName, previous);
}

void set_component_attr_type_cmd::redo()
{
  auto& registry = mRegistry.get();

  mPreviousValue =
      sys::get_component_attribute_value(registry, mComponentId, mAttributeName);
  sys::set_component_attribute_type(registry, mComponentId, mAttributeName, mNewType);
}

}  // namespace tactile