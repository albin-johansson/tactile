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

#include "component_factory.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "model/systems/validation_system.hpp"

namespace tactile::sys {

auto create_component(Model& model, ComponentSet& component_set, String name) -> Entity
{
  const auto definition_entity = model.create_entity();
  component_set.definitions.push_back(definition_entity);

  auto& definition = model.add<ComponentDefinition>(definition_entity);
  definition.name = std::move(name);

  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));
  return definition_entity;
}

}  // namespace tactile::sys
