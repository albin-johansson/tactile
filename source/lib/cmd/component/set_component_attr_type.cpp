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
#include "core/component.hpp"
#include "model/context.hpp"
#include "model/systems/component/component_def.hpp"
#include "model/systems/context/context_system.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

SetComponentAttrType::SetComponentAttrType(const Entity definition_entity,
                                           String attribute_name,
                                           const AttributeType new_type)
    : mDefinitionEntity {definition_entity},
      mAttributeName {std::move(attribute_name)},
      mNewType {new_type}
{
}

void SetComponentAttrType::undo()
{
  auto& model = get_global_model();

  auto& definition = model.get<ComponentDefinition>(mDefinitionEntity);
  definition.attributes[mAttributeName] = mOldValue.value();

  for (const auto& [component_entity, attribute_value]: mPrevAttributes) {
    auto& component = model.get<Component>(component_entity);
    component.attributes[mAttributeName] = attribute_value;
  }

  mOldValue.reset();
}

void SetComponentAttrType::redo()
{
  auto& model = get_global_model();

  const auto& definition = model.get<ComponentDefinition>(mDefinitionEntity);
  mOldValue = lookup_in(definition.attributes, mAttributeName);

  mPrevAttributes =
      sys::copy_single_attribute_in_components(model, mDefinitionEntity, mAttributeName);
  sys::force_update_component_attribute(model,
                                        mDefinitionEntity,
                                        mAttributeName,
                                        Attribute {mNewType});
}

auto SetComponentAttrType::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.change_comp_attr_type;
}

}  // namespace tactile::cmd