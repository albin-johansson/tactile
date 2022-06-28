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

#include "core/components/component_definition.hpp"
#include "core/components/component_index.hpp"
#include "misc/panic.hpp"

namespace tactile {

UpdateComponentAttrCmd::UpdateComponentAttrCmd(Shared<core::ComponentIndex> index,
                                               const UUID&                  componentId,
                                               std::string                  attribute,
                                               Attribute                    value)
    : ACommand{"Update Component Attribute"}
    , mIndex{index}
    , mComponentId{componentId}
    , mAttributeName{std::move(attribute)}
    , mUpdatedValue{std::move(value)}
{
  if (!mIndex) {
    throw TactileError{"Invalid null component index!"};
  }
}

void UpdateComponentAttrCmd::undo()
{
  auto& component = mIndex->at(mComponentId);
  component.update_attr(mAttributeName, mPreviousValue.value());
  mPreviousValue.reset();
}

void UpdateComponentAttrCmd::redo()
{
  auto& component = mIndex->at(mComponentId);
  mPreviousValue = component.get_attr(mAttributeName);
  component.update_attr(mAttributeName, mUpdatedValue);
}

auto UpdateComponentAttrCmd::merge_with(const ACommand& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const UpdateComponentAttrCmd&>(cmd);
    if (mComponentId == other.mComponentId && mAttributeName == other.mAttributeName) {
      mUpdatedValue = other.mUpdatedValue;
      return true;
    }
  }

  return false;
}

auto UpdateComponentAttrCmd::get_name() const -> const char*
{
  return "Update Component Attribute";
}

}  // namespace tactile