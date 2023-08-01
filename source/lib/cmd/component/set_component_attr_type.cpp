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

#include "set_component_attr_type.hpp"

#include <utility>  // move

#include "common/util/lookup.hpp"
#include "core/debug/assert.hpp"
#include "model/components/component_components.hpp"
#include "model/components/component_def_system.hpp"
#include "model/contexts/context_system.hpp"
#include "model/entity_validation.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

SetComponentAttrType::SetComponentAttrType(Registry* registry,
                                           const Entity component_entity,
                                           String attribute_name,
                                           const AttributeType new_type)
    : mRegistry {registry},
      mComponentEntity {component_entity},
      mAttributeName {std::move(attribute_name)},
      mNewType {new_type}
{
  TACTILE_ASSERT(sys::is_component_entity(*mRegistry, mComponentEntity));
}

void SetComponentAttrType::undo()
{
  auto& registry = *mRegistry;

  auto& component = registry.get<Component>(mComponentEntity);
  component.attributes[mAttributeName] = mOldValue.value();

  for (const auto& [attached_component_entity, attribute_value]: mPrevAttributes) {
    auto& attached_component = registry.get<AttachedComponent>(attached_component_entity);
    attached_component.attributes[mAttributeName] = attribute_value;
  }

  mOldValue.reset();
}

void SetComponentAttrType::redo()
{
  auto& registry = *mRegistry;

  const auto& component = registry.get<Component>(mComponentEntity);
  mOldValue = lookup_in(component.attributes, mAttributeName);

  mPrevAttributes = sys::copy_single_attribute_in_components(registry,
                                                             mComponentEntity,
                                                             mAttributeName);
  sys::force_update_component_attribute(registry,
                                        mComponentEntity,
                                        mAttributeName,
                                        Attribute {mNewType});
}

auto SetComponentAttrType::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.change_comp_attr_type;
}

}  // namespace tactile::cmd