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

#include "duplicate_component_attr.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "model/components/component_def_system.hpp"
#include "model/entity_validation.hpp"
#include "model/i18n/language_system.hpp"

namespace tactile::cmd {

DuplicateComponentAttr::DuplicateComponentAttr(Model* model,
                                               const Entity definition_entity,
                                               String attribute_name)
    : mModel {model},
      mDefinitionEntity {definition_entity},
      mAttributeName {std::move(attribute_name)}
{
  TACTILE_ASSERT(sys::is_component_definition_entity(*mModel, mDefinitionEntity));
}

void DuplicateComponentAttr::undo()
{
  auto& model = *mModel;

  sys::remove_component_attribute(model, mDefinitionEntity, mDuplicatedName.value());
  mDuplicatedName.reset();
}

void DuplicateComponentAttr::redo()
{
  auto& model = *mModel;

  mDuplicatedName =
      sys::duplicate_component_attribute(model, mDefinitionEntity, mAttributeName);
}

auto DuplicateComponentAttr::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.duplicate_comp_attr;
}

}  // namespace tactile::cmd