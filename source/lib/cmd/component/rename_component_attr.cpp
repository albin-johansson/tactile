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
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

RenameComponentAttr::RenameComponentAttr(Registry* registry,
                                         const Entity component_entity,
                                         String old_name,
                                         String new_name)
    : mRegistry {registry},
      mComponentEntity {component_entity},
      mOldName {std::move(old_name)},
      mNewName {std::move(new_name)}
{
  TACTILE_ASSERT(sys::is_component_entity(*mRegistry, mComponentEntity));
}

void RenameComponentAttr::undo()
{
  auto& registry = *mRegistry;
  sys::rename_component_attribute(registry, mComponentEntity, mNewName, mOldName);
}

void RenameComponentAttr::redo()
{
  auto& registry = *mRegistry;
  sys::rename_component_attribute(registry, mComponentEntity, mOldName, mNewName);
}

auto RenameComponentAttr::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.rename_comp_attr;
}

}  // namespace tactile::cmd