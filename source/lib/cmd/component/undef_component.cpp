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

#include "common/debug/assert.hpp"
#include "model/components/component_components.hpp"
#include "model/components/component_factory.hpp"
#include "model/components/component_set_system.hpp"
#include "model/contexts/attached_component_system.hpp"
#include "model/contexts/context_system.hpp"
#include "model/entity_validation.hpp"
#include "model/i18n/language_system.hpp"

namespace tactile::cmd {

UndefComponent::UndefComponent(Model* model,
                               const Entity component_set_entity,
                               String component_name)
    : mModel {model},
      mComponentSetEntity {component_set_entity},
      mComponentName {std::move(component_name)}
{
  TACTILE_ASSERT(sys::is_component_set_entity(*mModel, mComponentSetEntity));
}

void UndefComponent::undo()
{
  auto& model = *mModel;

  auto& component_set = model.get<ComponentSet>(mComponentSetEntity);
  const auto component_entity =
      sys::create_component(model, component_set, mComponentName);

  auto& component = model.get<Component>(component_entity);
  component.attributes = std::move(mPrevComponentValues.value());

  for (auto& [context_entity, attributes]: mRemovedComponentValues) {
    auto& context = model.get<Context>(context_entity);
    const auto attached_component_entity =
        sys::attach_component(model, context, component_entity);

    auto& attached_component = model.get<AttachedComponent>(attached_component_entity);
    attached_component.attributes = std::move(attributes);
  }

  mPrevComponentValues.reset();
  mRemovedComponentValues.clear();
}

void UndefComponent::redo()
{
  auto& model = *mModel;
  auto& component_set = model.get<ComponentSet>(mComponentSetEntity);

  const auto component_entity = sys::find_component(model, component_set, mComponentName);

  const auto& component = model.get<Component>(component_entity);
  mPrevComponentValues = component.attributes;

  mRemovedComponentValues = sys::copy_component_values(model, component_entity);
  sys::remove_component(model, component_set, mComponentName);
}

auto UndefComponent::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.undef_comp;
}

}  // namespace tactile::cmd