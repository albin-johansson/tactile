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

#include "update_component_attr_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

update_component_attr_cmd::update_component_attr_cmd(registry_ref registry,
                                                     const component_id id,
                                                     std::string attribute,
                                                     attribute_value value)
    : ACommand{"update Component Attribute"}
    , mRegistry{registry}
    , mComponentId{id}
    , mAttributeName{std::move(attribute)}
    , mUpdatedValue{std::move(value)}
{}

void update_component_attr_cmd::undo()
{
  auto& registry = mRegistry.get();
  sys::set_component_attribute_value(registry,
                                     mComponentId,
                                     mAttributeName,
                                     mPreviousValue.value());
}

void update_component_attr_cmd::redo()
{
  auto& registry = mRegistry.get();

  mPreviousValue =
      sys::get_component_attribute_value(registry, mComponentId, mAttributeName);
  sys::set_component_attribute_value(registry,
                                     mComponentId,
                                     mAttributeName,
                                     mUpdatedValue);
}

auto update_component_attr_cmd::merge_with(const ACommand& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const update_component_attr_cmd&>(cmd);
    if (mComponentId == other.mComponentId && mAttributeName == other.mAttributeName) {
      mUpdatedValue = other.mUpdatedValue;
      return true;
    }
  }

  return false;
}

}  // namespace tactile