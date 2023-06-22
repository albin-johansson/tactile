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

#include "rename_component_attr.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "model/components/component_def_system.hpp"
#include "model/entity_validation.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

RenameComponentAttr::RenameComponentAttr(Model* model,
                                         const Entity definition_entity,
                                         String old_name,
                                         String new_name)
    : mModel {model},
      mComponentDefinitionEntity {definition_entity},
      mOldName {std::move(old_name)},
      mNewName {std::move(new_name)}
{
  TACTILE_ASSERT(
      sys::is_component_definition_entity(*mModel, mComponentDefinitionEntity));
}

void RenameComponentAttr::undo()
{
  auto& model = *mModel;
  sys::rename_component_attribute(model, mComponentDefinitionEntity, mNewName, mOldName);
}

void RenameComponentAttr::redo()
{
  auto& model = *mModel;
  sys::rename_component_attribute(model, mComponentDefinitionEntity, mOldName, mNewName);
}

auto RenameComponentAttr::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.rename_comp_attr;
}

}  // namespace tactile::cmd