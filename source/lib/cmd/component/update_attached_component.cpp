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

#include "update_attached_component.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "common/util/lookup.hpp"
#include "model/components/component_components.hpp"
#include "model/entity_validation.hpp"
#include "model/i18n/language_system.hpp"

namespace tactile::cmd {

UpdateAttachedComponent::UpdateAttachedComponent(Model* model,
                                                 const Entity component_entity,
                                                 String attribute_name,
                                                 Attribute new_value)
    : mModel {model},
      mComponentEntity {component_entity},
      mAttributeName {std::move(attribute_name)},
      mNewValue {std::move(new_value)}
{
  TACTILE_ASSERT(sys::is_component_entity(*mModel, mComponentEntity));
}

void UpdateAttachedComponent::undo()
{
  auto& model = *mModel;

  auto& component = model.get<Component>(mComponentEntity);
  component.attributes[mAttributeName] = mOldValue.value();

  mOldValue.reset();
}

void UpdateAttachedComponent::redo()
{
  auto& model = *mModel;
  auto& component = model.get<Component>(mComponentEntity);

  mOldValue = lookup_in(component.attributes, mAttributeName);
  component.attributes[mAttributeName] = mNewValue;
}

auto UpdateAttachedComponent::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const UpdateAttachedComponent*>(cmd)) {
    const bool can_merge = mComponentEntity == other->mComponentEntity &&
                           mAttributeName == other->mAttributeName;
    if (can_merge) {
      mNewValue = other->mNewValue;
      return true;
    }
  }

  return false;
}

auto UpdateAttachedComponent::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.update_comp_attr;
}

}  // namespace tactile::cmd