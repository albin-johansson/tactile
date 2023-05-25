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

#include "undef_component.hpp"

#include <utility>  // move

#include "core/component.hpp"
#include "model/context.hpp"
#include "model/systems/component/component_set.hpp"
#include "model/systems/context/components.hpp"
#include "model/systems/context/context_system.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

UndefComponent::UndefComponent(const Entity component_set_entity, String component_name)
    : mComponentSetEntity {component_set_entity},
      mComponentName {std::move(component_name)}
{
}

void UndefComponent::undo()
{
  auto& model = get_global_model();

  auto& component_set = model.get<ComponentSet>(mComponentSetEntity);
  const auto definition_entity =
      sys::create_component(model, component_set, mComponentName);

  auto& definition = model.get<ComponentDefinition>(definition_entity);
  definition.attributes = std::move(mPrevDefinitionValues.value());

  for (auto& [context_entity, attributes]: mRemovedComponentValues) {
    auto& context = model.get<Context>(context_entity);
    const auto component_entity =
        sys::attach_component(model, context, definition_entity);

    auto& component = model.get<Component>(component_entity);
    component.attributes = std::move(attributes);
  }

  mPrevDefinitionValues.reset();
  mRemovedComponentValues.clear();
}

void UndefComponent::redo()
{
  auto& model = get_global_model();
  auto& component_set = model.get<ComponentSet>(mComponentSetEntity);

  const auto definition_entity =
      sys::find_component_definition(model, component_set, mComponentName);

  const auto& definition = model.get<ComponentDefinition>(definition_entity);
  mPrevDefinitionValues = definition.attributes;

  mRemovedComponentValues = sys::copy_component_values(model, definition_entity);
  sys::remove_component(model, component_set, mComponentName);
}

auto UndefComponent::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.undef_comp;
}

}  // namespace tactile::cmd