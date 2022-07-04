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

#include "update_component_cmd.hpp"

#include <utility>  // move

#include "core/components/component_index.hpp"
#include "misc/panic.hpp"

namespace tactile {

UpdateComponentCmd::UpdateComponentCmd(Shared<core::ComponentIndex> index,
                                       const UUID&                  componentId,
                                       std::string                  attribute,
                                       Attribute                    value)
    : mIndex{index}
    , mComponentId{componentId}
    , mAttributeName{std::move(attribute)}
    , mUpdatedValue{std::move(value)}
{
  if (!mIndex) {
    throw TactileError{"Invalid null component index!"};
  }
}

void UpdateComponentCmd::undo()
{
  auto& component = mIndex->at(mComponentId);
  component.update_attr(mAttributeName, mPreviousValue.value());
  mPreviousValue.reset();
}

void UpdateComponentCmd::redo()
{
  auto& component = mIndex->at(mComponentId);
  mPreviousValue = component.get_attr(mAttributeName);
  component.update_attr(mAttributeName, mUpdatedValue);
}

auto UpdateComponentCmd::merge_with(const ICommand* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const UpdateComponentCmd*>(cmd)) {
    if (mComponentId == other->mComponentId && mAttributeName == other->mAttributeName) {
      mUpdatedValue = other->mUpdatedValue;
      return true;
    }
  }

  return false;
}

auto UpdateComponentCmd::get_name() const -> const char*
{
  return "Update Component Attribute";
}

}  // namespace tactile