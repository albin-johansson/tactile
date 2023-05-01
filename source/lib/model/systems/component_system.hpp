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

#include "common/type/ecs.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/maybe.hpp"
#include "common/type/string.hpp"
#include "common/type/string_map.hpp"
#include "core/attribute.hpp"
#include "core/component.hpp"
#include "model/model.hpp"

namespace tactile::sys {

/// Indicates whether an entity is a valid component set entity.
[[nodiscard]] auto is_component_set_entity(const Model& model, Entity entity) -> bool;

/// Indicates whether an entity is a valid component definition entity.
[[nodiscard]] auto is_component_definition_entity(const Model& model, Entity entity)
    -> bool;

/// Indicates whether an entity is a valid component entity.
[[nodiscard]] auto is_component_entity(const Model& model, Entity entity) -> bool;

/// Creates a new empty component definition.
auto create_component(Model& model, Entity component_set_entity, String name) -> Entity;

/// Removes a component definition (and destroys it), and removes it from all contexts.
void remove_component(Model& model, Entity component_set_entity, StringView name);

/// Tries to find a component definition entity with a specific name, or returns null.
[[nodiscard]] auto find_component_definition(const Model& model,
                                             const ComponentSet& component_set,
                                             StringView name) -> Entity;

/// Creates a new component entity based on a component definition template.
[[nodiscard]] auto instantiate_component(Model& model, Entity definition_entity)
    -> Entity;

/// Copies an existing component instance.
[[nodiscard]] auto copy_component(Model& model, Entity component_entity) -> Entity;

/// Adds an attribute to a component definition, and updates all components.
void add_component_attribute(Model& model,
                             Entity definition_entity,
                             const String& name,
                             const Attribute& value);

/// Removes an attribute from a component definition, and updates all components.
void remove_component_attribute(Model& model,
                                Entity definition_entity,
                                StringView attribute_name);

/// Renames an attribute in a component definition, and updates all components.
void rename_component_attribute(Model& model,
                                Entity definition_entity,
                                StringView old_attribute_name,
                                const String& new_attribute_name);

/// Duplicate an attribute in a component definition, and updates all components.
auto duplicate_component_attribute(Model& model,
                                   Entity definition_entity,
                                   StringView attribute_name) -> String;

/// Updates the default value of a component attribute, and updates all components.
void force_update_component_attribute(Model& model,
                                      Entity definition_entity,
                                      const String& attribute_name,
                                      const Attribute& attribute_value);

/// Resets the attribute values in a component to the component definition defaults.
auto reset_component_values(Model& model, Entity component_entity)
    -> StringMap<Attribute>;

/// Creates a component entity and attaches it to the specified context.
auto attach_component(Model& model, Entity context_entity, Entity definition_entity)
    -> Entity;

/// Detaches a component from a context (this destroys the attached component entity).
auto detach_component(Model& model, Entity context_entity, Entity definition_entity)
    -> Maybe<StringMap<Attribute>>;

/// Copies a specific attribute in all components of a specific component type.
[[nodiscard]] auto copy_single_attribute_in_components(
    const Model& model,
    ComponentDefinitionEntity def_entity,
    StringView attribute_name) -> HashMap<ComponentEntity, Attribute>;

/// Copies the attributes of all components of a specific component type.
[[nodiscard]] auto copy_component_values(const Model& model,
                                         ComponentDefinitionEntity definition_entity)
    -> HashMap<ContextEntity, StringMap<Attribute>>;

}  // namespace tactile::sys
