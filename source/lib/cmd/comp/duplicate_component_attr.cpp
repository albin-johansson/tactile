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

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/model.hpp"
#include "model/systems/component_system.hpp"

namespace tactile::cmd {

DuplicateComponentAttr::DuplicateComponentAttr(const Entity definition_entity,
                                               String attribute_name)
    : mComponentDefinitionEntity {definition_entity},
      mAttributeName {std::move(attribute_name)}
{
}

void DuplicateComponentAttr::undo()
{
  auto& model = get_model();
  sys::remove_component_attribute(model,
                                  mComponentDefinitionEntity,
                                  mDuplicatedName.value());
  mDuplicatedName.reset();
}

void DuplicateComponentAttr::redo()
{
  auto& model = get_model();
  mDuplicatedName = sys::duplicate_component_attribute(model,
                                                       mComponentDefinitionEntity,
                                                       mAttributeName);
}

auto DuplicateComponentAttr::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.duplicate_comp_attr;
}

}  // namespace tactile::cmd