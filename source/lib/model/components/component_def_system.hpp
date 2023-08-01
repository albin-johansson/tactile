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

#pragma once

#include "common/attribute.hpp"
#include "core/ecs/entity.hpp"
#include "core/containers/hash_map.hpp"
#include "core/containers/string.hpp"
#include "core/containers/string_map.hpp"
#include "model/registry.hpp"

namespace tactile::sys {

/// Creates a new attached component entity based on a component definition template.
[[nodiscard]] auto instantiate_component(Registry& registry, Entity component_entity)
    -> Entity;

/// Adds an attribute to a component definition, and updates all components.
void add_component_attribute(Registry& registry,
                             Entity component_entity,
                             const String& name,
                             const Attribute& value);

/// Removes an attribute from a component definition, and updates all components.
void remove_component_attribute(Registry& registry,
                                Entity component_entity,
                                StringView attribute_name);

/// Renames an attribute in a component definition, and updates all components.
void rename_component_attribute(Registry& registry,
                                Entity component_entity,
                                StringView old_attribute_name,
                                const String& new_attribute_name);

/// Duplicate an attribute in a component definition, and updates all components.
auto duplicate_component_attribute(Registry& registry,
                                   Entity component_entity,
                                   StringView attribute_name) -> String;

/// Updates the default value of a component attribute, and updates all components.
void force_update_component_attribute(Registry& registry,
                                      Entity component_entity,
                                      const String& attribute_name,
                                      const Attribute& attribute_value);

}  // namespace tactile::sys
