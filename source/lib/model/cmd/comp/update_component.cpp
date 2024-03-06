/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "update_component.hpp"

#include <utility>  // move

#include "core/component/component_index.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

UpdateComponent::UpdateComponent(Shared<ComponentIndex> index,
                                 const UUID& component_id,
                                 String attribute,
                                 Attribute value)
    : mIndex {std::move(index)},
      mComponentId {component_id},
      mAttributeName {std::move(attribute)},
      mUpdatedValue {std::move(value)}
{
  if (!mIndex) {
    throw Exception {"Invalid null component index!"};
  }
}

void UpdateComponent::undo()
{
  auto& component_def = mIndex->get_comp(mComponentId);
  component_def.update_attr(mAttributeName, mPreviousValue.value());

  mPreviousValue.reset();
}

void UpdateComponent::redo()
{
  auto& component_def = mIndex->get_comp(mComponentId);

  mPreviousValue = component_def.get_attr(mAttributeName);
  component_def.update_attr(mAttributeName, mUpdatedValue);
}

auto UpdateComponent::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const UpdateComponent*>(cmd)) {
    if (mComponentId == other->mComponentId && mAttributeName == other->mAttributeName) {
      mUpdatedValue = other->mUpdatedValue;
      return true;
    }
  }

  return false;
}

auto UpdateComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.update_comp_attr_defaults;
}

}  // namespace tactile::cmd
