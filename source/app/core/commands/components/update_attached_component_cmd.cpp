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

#include "update_attached_component_cmd.hpp"

#include <utility>  // move

#include "core/components/component.hpp"
#include "core/components/component_bundle.hpp"
#include "core/contexts/context.hpp"
#include "misc/panic.hpp"

namespace tactile {

UpdateAttachedComponentCmd::UpdateAttachedComponentCmd(Shared<IContext> context,
                                                       const UUID&      componentId,
                                                       std::string      attribute,
                                                       Attribute        value)
    : mContext {std::move(context)}
    , mComponentId {componentId}
    , mAttributeName {std::move(attribute)}
    , mUpdatedValue {std::move(value)}
{
  if (!mContext) {
    throw TactileError {"Invalid null context!"};
  }
}

void UpdateAttachedComponentCmd::undo()
{
  auto& comps = mContext->get_comps();
  auto& component = comps.at(mComponentId);

  component.update_attr(mAttributeName, mPreviousValue.value());
  mPreviousValue.reset();
}

void UpdateAttachedComponentCmd::redo()
{
  auto& comps = mContext->get_comps();
  auto& component = comps.at(mComponentId);

  mPreviousValue = component.get_attr(mAttributeName);
  component.update_attr(mAttributeName, mUpdatedValue);
}

auto UpdateAttachedComponentCmd::merge_with(const ICommand* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const UpdateAttachedComponentCmd*>(cmd)) {
    const bool canMerge = mContext->get_uuid() == other->mContext->get_uuid() &&
                          mComponentId == other->mComponentId &&
                          mAttributeName == other->mAttributeName;
    if (canMerge) {
      mUpdatedValue = other->mUpdatedValue;
      return true;
    }
  }

  return false;
}

auto UpdateAttachedComponentCmd::get_name() const -> const char*
{
  return "Update Component Attribute";
}

}  // namespace tactile