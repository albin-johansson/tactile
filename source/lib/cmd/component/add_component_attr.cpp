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

#include "add_component_attr.hpp"

#include <utility>  // move

#include "core/debug/assert.hpp"
#include "model/components/component_def_system.hpp"
#include "model/entity_validation.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

AddComponentAttr::AddComponentAttr(Registry* registry,
                                   const Entity component_entity,
                                   String name)
    : mRegistry {registry},
      mComponentEntity {component_entity},
      mAttributeName {std::move(name)}
{
  TACTILE_ASSERT(sys::is_component_entity(*mRegistry, mComponentEntity));
}

void AddComponentAttr::undo()
{
  auto& registry = *mRegistry;
  sys::remove_component_attribute(registry, mComponentEntity, mAttributeName);
}

void AddComponentAttr::redo()
{
  auto& registry = *mRegistry;
  sys::add_component_attribute(registry,
                               mComponentEntity,
                               mAttributeName,
                               Property {PropertyType::Str});
}

auto AddComponentAttr::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.create_comp_attr;
}

}  // namespace tactile::cmd
