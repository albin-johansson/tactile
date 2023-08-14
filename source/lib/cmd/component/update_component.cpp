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

#include "model/components/component_components.hpp"
#include "model/entity_validation.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"
#include "tactile/core/common/assert.hpp"
#include "tactile/core/containers/lookup.hpp"

namespace tactile::cmd {

UpdateComponent::UpdateComponent(Registry* registry,
                                 const Entity component_entity,
                                 String attribute_name,
                                 Property new_value)
    : mRegistry {registry},
      mComponentEntity {component_entity},
      mAttributeName {std::move(attribute_name)},
      mNewValue {std::move(new_value)}
{
  TACTILE_ASSERT(sys::is_component_entity(*mRegistry, mComponentEntity));
}

void UpdateComponent::undo()
{
  auto& registry = *mRegistry;

  auto& component = registry.get<Component>(mComponentEntity);
  component.attributes[mAttributeName] = mOldValue.value();

  mOldValue.reset();
}

void UpdateComponent::redo()
{
  auto& registry = *mRegistry;
  auto& component = registry.get<Component>(mComponentEntity);

  mOldValue = lookup_in(component.attributes, mAttributeName);
  component.attributes[mAttributeName] = mNewValue;
}

auto UpdateComponent::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const UpdateComponent*>(cmd)) {
    if (mComponentEntity == other->mComponentEntity &&
        mAttributeName == other->mAttributeName) {
      mNewValue = other->mNewValue;
      return true;
    }
  }

  return false;
}

auto UpdateComponent::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.update_comp_attr_defaults;
}

}  // namespace tactile::cmd