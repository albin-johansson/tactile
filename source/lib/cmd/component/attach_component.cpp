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

#include "attach_component.hpp"

#include "core/debug/assert.hpp"
#include "model/components/component_def_system.hpp"
#include "model/contexts/context_components.hpp"
#include "model/entity_validation.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

AttachComponent::AttachComponent(Registry* registry,
                                 const Entity context_entity,
                                 const Entity component_entity)
    : mRegistry {registry},
      mContextEntity {context_entity},
      mComponentEntity {component_entity}
{
  TACTILE_ASSERT(sys::is_context_entity(*mRegistry, mContextEntity));
  TACTILE_ASSERT(sys::is_component_entity(*mRegistry, mComponentEntity));
}

void AttachComponent::undo()
{
  auto& registry = *mRegistry;

  auto& context = registry.get<Context>(mContextEntity);
  std::erase(context.comps, mAttachedComponentEntity);
}

void AttachComponent::redo()
{
  auto& registry = *mRegistry;

  if (mAttachedComponentEntity == kNullEntity) {
    mAttachedComponentEntity = sys::instantiate_component(registry, mComponentEntity);
  }

  auto& context = registry.get<Context>(mContextEntity);
  context.comps.push_back(mAttachedComponentEntity);
}

auto AttachComponent::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.attach_comp;
}

}  // namespace tactile::cmd