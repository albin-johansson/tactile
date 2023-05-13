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

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/model.hpp"
#include "model/systems/component/component.hpp"

namespace tactile::cmd {

ResetAttachedComponent::ResetAttachedComponent(const Entity component_entity)
    : mComponentEntity {component_entity}
{
}

void ResetAttachedComponent::undo()
{
  auto& model = get_model();

  auto& component = model.get<Component>(mComponentEntity);
  component.attributes = std::move(mPreviousValues.value());

  mPreviousValues.reset();
}

void ResetAttachedComponent::redo()
{
  auto& model = get_model();
  auto& component = model.get<Component>(mComponentEntity);
  mPreviousValues = sys::reset_component_values(model, component);
}

auto ResetAttachedComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.reset_comp;
}

}  // namespace tactile::cmd