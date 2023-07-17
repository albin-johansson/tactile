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

#include "detach_component.hpp"

#include "common/debug/assert.hpp"
#include "model/components/component_components.hpp"
#include "model/contexts/attached_component_system.hpp"
#include "model/entity_validation.hpp"
#include "model/i18n/language_system.hpp"

namespace tactile::cmd {

DetachComponent::DetachComponent(Registry* registry,
                                 const Entity context_entity,
                                 const Entity component_entity)
    : mRegistry {registry},
      mContextEntity {context_entity},
      mComponentEntity {component_entity}
{
  TACTILE_ASSERT(sys::is_context_entity(*mRegistry, mContextEntity));
  TACTILE_ASSERT(sys::is_component_entity(*mRegistry, mComponentEntity));
}

void DetachComponent::undo()
{
  auto& registry = *mRegistry;
  auto& context = registry.get<Context>(mContextEntity);

  const auto attached_component_entity =
      sys::attach_component(registry, context, mComponentEntity);
  auto& attached_component = registry.get<AttachedComponent>(attached_component_entity);
  attached_component.attributes = mPrevValues.value();

  mPrevValues.reset();
}

void DetachComponent::redo()
{
  auto& registry = *mRegistry;

  auto& context = registry.get<Context>(mContextEntity);
  mPrevValues = sys::detach_component(registry, context, mComponentEntity);
}

auto DetachComponent::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mRegistry);
  return strings.cmd.detach_comp;
}

}  // namespace tactile::cmd