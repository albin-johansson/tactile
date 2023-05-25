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

#include "common/util/lookup.hpp"
#include "core/component.hpp"
#include "model/context.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

UpdateComponent::UpdateComponent(const Entity definition_entity,
                                 String attribute_name,
                                 Attribute new_value)
    : mDefinitionEntity {definition_entity},
      mAttributeName {std::move(attribute_name)},
      mNewValue {std::move(new_value)}
{
}

void UpdateComponent::undo()
{
  auto& model = get_global_model();

  auto& definition = model.get<ComponentDefinition>(mDefinitionEntity);
  definition.attributes[mAttributeName] = mOldValue.value();

  mOldValue.reset();
}

void UpdateComponent::redo()
{
  auto& model = get_global_model();
  auto& definition = model.get<ComponentDefinition>(mDefinitionEntity);

  mOldValue = lookup_in(definition.attributes, mAttributeName);
  definition.attributes[mAttributeName] = mNewValue;
}

auto UpdateComponent::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const UpdateComponent*>(cmd)) {
    if (mDefinitionEntity == other->mDefinitionEntity &&
        mAttributeName == other->mAttributeName) {
      mNewValue = other->mNewValue;
      return true;
    }
  }

  return false;
}

auto UpdateComponent::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.update_comp_attr_defaults;
}

}  // namespace tactile::cmd