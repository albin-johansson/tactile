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

#include "core/component.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/context.hpp"
#include "model/systems/context/components.hpp"

namespace tactile::cmd {

DetachComponent::DetachComponent(const Entity context_entity,
                                 const Entity definition_entity)
    : mContextEntity {context_entity},
      mDefinitionEntity {definition_entity}
{
}

void DetachComponent::undo()
{
  auto& model = get_global_model();
  auto& context = model.get<Context>(mContextEntity);

  const auto component_entity = sys::attach_component(model, context, mDefinitionEntity);
  auto& component = model.get<Component>(component_entity);
  component.attributes = mPrevValues.value();

  mPrevValues.reset();
}

void DetachComponent::redo()
{
  auto& model = get_global_model();
  auto& context = model.get<Context>(mContextEntity);
  mPrevValues = sys::detach_component(model, context, mDefinitionEntity);
}

auto DetachComponent::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.detach_comp;
}

}  // namespace tactile::cmd