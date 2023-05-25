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

#include "remove_component_attr.hpp"

#include <utility>  // move

#include "components/component.hpp"
#include "model/context.hpp"
#include "model/systems/component/component_def.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

RemoveComponentAttr::RemoveComponentAttr(const Entity definition_entity, String attribute)
    : mComponentDefinitionEntity {definition_entity},
      mAttributeName {std::move(attribute)}
{
}

void RemoveComponentAttr::undo()
{
  auto& model = get_global_model();
  sys::add_component_attribute(model,
                               mComponentDefinitionEntity,
                               mAttributeName,
                               mPreviousValue.value());
  mPreviousValue.reset();
}

void RemoveComponentAttr::redo()
{
  auto& model = get_global_model();

  auto& definition = model.get<ComponentDefinition>(mComponentDefinitionEntity);
  mPreviousValue = definition.attributes.at(mAttributeName);

  sys::remove_component_attribute(model, mComponentDefinitionEntity, mAttributeName);
}

auto RemoveComponentAttr::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.remove_comp_attr;
}

}  // namespace tactile::cmd