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

#include "reset_attached_component.hpp"

#include <utility>  // move

#include "core/debug/assert.hpp"
#include "model/components/attached_component_system.hpp"
#include "model/components/component_components.hpp"
#include "model/entity_validation.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

ResetAttachedComponent::ResetAttachedComponent(Registry* registry,
                                               const Entity attached_component_entity)
    : mRegistry {registry},
      mAttachedComponentEntity {attached_component_entity}
{
  TACTILE_ASSERT(sys::is_attached_component_entity(*mRegistry, mAttachedComponentEntity));
}

void ResetAttachedComponent::undo()
{
  auto& registry = *mRegistry;

  auto& attached_component = registry.get<AttachedComponent>(mAttachedComponentEntity);
  attached_component.attributes = std::move(mPreviousValues.value());

  mPreviousValues.reset();
}

void ResetAttachedComponent::redo()
{
  auto& registry = *mRegistry;

  auto& attached_component = registry.get<AttachedComponent>(mAttachedComponentEntity);
  mPreviousValues = sys::reset_component_values(registry, attached_component);
}

auto ResetAttachedComponent::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.reset_comp;
}

}  // namespace tactile::cmd