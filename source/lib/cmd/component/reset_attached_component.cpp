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

#include "common/debug/assert.hpp"
#include "model/components/component_components.hpp"
#include "model/components/component_system.hpp"
#include "model/entity_validation.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

ResetAttachedComponent::ResetAttachedComponent(Model* model,
                                               const Entity component_entity)
    : mModel {model},
      mComponentEntity {component_entity}
{
  TACTILE_ASSERT(sys::is_component_entity(*mModel, mComponentEntity));
}

void ResetAttachedComponent::undo()
{
  auto& model = *mModel;

  auto& component = model.get<Component>(mComponentEntity);
  component.attributes = std::move(mPreviousValues.value());

  mPreviousValues.reset();
}

void ResetAttachedComponent::redo()
{
  auto& model = *mModel;

  auto& component = model.get<Component>(mComponentEntity);
  mPreviousValues = sys::reset_component_values(model, component);
}

auto ResetAttachedComponent::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.reset_comp;
}

}  // namespace tactile::cmd