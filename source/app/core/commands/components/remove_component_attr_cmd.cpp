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

#include "core/components/component_index.hpp"
#include "misc/panic.hpp"

namespace tactile {

RemoveComponentAttrCmd::RemoveComponentAttrCmd(Shared<ComponentIndex> index,
                                               const UUID&            componentId,
                                               std::string            attribute)
    : mIndex{std::move(index)}
    , mComponentId{componentId}
    , mAttributeName{std::move(attribute)}
{
  if (!mIndex) {
    throw TactileError{"Invalid null component index!"};
  }
}

void RemoveComponentAttrCmd::undo()
{
  auto& definition = mIndex->at(mComponentId);
  definition.add_attr(mAttributeName, mPreviousValue.value());
  mPreviousValue.reset();
}

void RemoveComponentAttrCmd::redo()
{
  auto& definition = mIndex->at(mComponentId);
  mPreviousValue = definition.get_attr(mAttributeName);
  definition.remove_attr(mAttributeName);
}

auto RemoveComponentAttrCmd::get_name() const -> const char*
{
  return "Remove Component Attribute";
}

}  // namespace tactile